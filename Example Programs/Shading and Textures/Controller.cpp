#include "Controller.h"
#include "FADirectXException.h"
#include <iomanip>
#include <iostream>
#include <sstream>

namespace MVC
{
	Controller::Controller() : mModel{ nullptr }, mView{ nullptr }
	{}

	Controller::Controller(Model* model, View* view) : mModel{ model }, mView{ view }
	{}

	Model* Controller::GetModel()
	{
		return mModel;
	}

	View* Controller::GetView()
	{
		return mView;
	}

	void Controller::SetModel(Model* model)
	{
		mModel = model;
	}

	void Controller::SetView(View* view)
	{
		mView = view;
	}

	void Controller::FrameStats()
	{
		//computes average frames per second and
		//the average time it takes to show each frame

		static unsigned int frameCount{ 0 };
		static float timeElapsed{ 0.0f };

		++frameCount;

		timeElapsed += mModel->GetFrameTime().GetDeltaTime();
		if (timeElapsed >= 1.0f)
		{
			float fps = (float)frameCount; //fps = number of frames / 1 second
			float milliSecondsPerFrame = 1000.0f / fps;

			std::stringstream fpsStream;
			fpsStream << std::setprecision(6) << fps;
			std::string fpsString{ fpsStream.str() };

			std::stringstream milliSecondsPerFrameStream;
			milliSecondsPerFrameStream << std::setprecision(6) << milliSecondsPerFrame;
			std::string milliSecondsPerFrameString{ milliSecondsPerFrameStream.str() };

			std::wstring fpsWString{ AnsiToWString(fpsString) };
			std::wstring milliSecondsPerFrameWString{ AnsiToWString(milliSecondsPerFrameString) };
			std::wstring textStr = L"FPS: " + fpsWString + L"     Frame Time: " + milliSecondsPerFrameWString;
			SetWindowText(mView->GetMainWindow().GetWindowHandle(), textStr.c_str());

			//reset for next average
			frameCount = 0;
			timeElapsed = 0.0f;
		}
	}

	void Controller::UserInput()
	{
		if (mView->GetCameraMovement())
		{
			mModel->GetCamera().KeyboardInputWASD(mModel->GetFrameTime().GetDeltaTime());
				mModel->GetCamera().MouseInput();
		}
	}

	void Controller::Update()
	{
		mModel->GetCamera().UpdateViewMatrix();
		mModel->GetPerspectiveProjection().UpdateProjectionMatrix();

		PassConstantBuffer passConstantsData;
		passConstantsData.passConstants.view = Transpose(mModel->GetCamera().GetViewMatrix());
		passConstantsData.passConstants.projection = Transpose(mModel->GetPerspectiveProjection().GetProjectionMatrix());
		passConstantsData.passConstants.cameraPosition = mModel->GetCamera().GetCameraPosition();

		mModel->GetScene()->CopyDataIntoDynamicBuffer(PASSCB, 0, &passConstantsData, sizeof(PassConstantBuffer));

		static float angularVelocity{ 45.0f };

		LightConstantBuffer lights;

		//Copy the point lights that are being displayed.
		unsigned int numLightSourcesToCopy = mView->GetCurrentLightSource() + 1;
		for (unsigned int i = 0; i < numLightSourcesToCopy; ++i)
		{
			lights.lightSources[i] = mModel->GetLightSources().at(i);
		}

		//Change the light color to black for the point lights that aren't being displayed. 
		for (unsigned int i = numLightSourcesToCopy; i < MAX_NUM_LIGHTS; ++i)
		{
			lights.lightSources[i].color = FAColor::Color(0.0f, 0.0f, 0.0f, 1.0f);
		}

		mModel->GetScene()->CopyDataIntoDynamicBuffer(LIGHTCB, 0, &lights, sizeof(LightConstantBuffer));

		if (mView->GetRotateShape())
		{
			mModel->GetShape(mView->GetCurrentShape())->SetOrientation(FAMath::Normalize(
				FAMath::RotationQuaternion(angularVelocity * mModel->GetFrameTime().GetDeltaTime(), FAMath::Vector3D(0.0f, 1.0f, 0.0f)) *
				mModel->GetShape(mView->GetCurrentShape())->GetOrientation()));
		}

		mModel->box.UpdateModelMatrix();
		mModel->pyramid.UpdateModelMatrix();
		mModel->sphere.UpdateModelMatrix();
		mModel->cylinder.UpdateModelMatrix();
		mModel->cone.UpdateModelMatrix();

		ObjectConstantBuffer objectConstantData;
		FAMath::Matrix4x4 modelTrans(mModel->GetShape(mView->GetCurrentShape())->GetModelMatrix());
		objectConstantData.objectConstants.localToWorld = Transpose(modelTrans);
		objectConstantData.objectConstants.inverseTransposeLocalToWorld = Inverse(modelTrans);
		objectConstantData.objectConstants.color = mModel->GetShape(mView->GetCurrentShape())->GetColor();

		mModel->GetShape(mView->GetCurrentShape())->UpdateShape(mModel->GetScene(), &objectConstantData, sizeof(ObjectConstantBuffer));

		for (unsigned int i = 0; i < MAX_NUM_LIGHTS; ++i)
		{
			if (mView->GetRotatePointLight())
			{
				if (i == 0 || i == 1)
				{
					mModel->GetLightSources().at(i).position = 
						FAMath::Rotate(FAMath::RotationQuaternion(angularVelocity * mModel->GetFrameTime().GetDeltaTime(), 0.0f, 1.0f, 0.0f),
						mModel->GetLightSources().at(i).position);

					mModel->GetPointLight(i).GetShape().SetPosition(mModel->GetLightSources().at(i).position);
				}
				else
				{
					mModel->GetLightSources().at(i).position =
						FAMath::Rotate(FAMath::RotationQuaternion(angularVelocity * mModel->GetFrameTime().GetDeltaTime(), 1.0f, 0.0f, 0.0f),
							mModel->GetLightSources().at(i).position);

					mModel->GetPointLight(i).GetShape().SetPosition(mModel->GetLightSources().at(i).position);
				}
			}

			mModel->GetPointLight(i).UpdateModelMatrix();

			ObjectConstantBuffer pointLightConstantData;
			FAMath::Matrix4x4 modelTrans(mModel->GetPointLight(i).GetShape().GetModelMatrix());
			pointLightConstantData.objectConstants.localToWorld = Transpose(modelTrans);
			pointLightConstantData.objectConstants.inverseTransposeLocalToWorld = Inverse(modelTrans);
			pointLightConstantData.objectConstants.color = mModel->GetPointLight(i).GetShape().GetColor();

			mModel->GetPointLight(i).GetShape().UpdateShape(mModel->GetScene(), &pointLightConstantData, sizeof(ObjectConstantBuffer));
		}
	}

	void Controller::Draw()
	{
		FARender::RenderScene* scene{ mModel->GetScene() };

		//All the commands needed before rendering the shapes
		scene->BeforeRenderObjects();

		scene->LinkTextureViewHeap();

		if (mView->GetCurrentRenderOption() == TEXTURES_PLUS_SHADING)
		{
			if (mView->GetCurrentTexture() == EARTH)
			{
				scene->LinkPSOAndRootSignature(EARTH_SHADING_PSO, EARTH_SHADING_ROOT_SIG);

				//Link the textures to the pipeline.
				scene->LinkTexture(4, 2);
			}
			else //current texture is moon or sun
			{
				scene->LinkPSOAndRootSignature(MOON_SUN_SHADING_PSO, MOON_SUN_SHADING_ROOT_SIG);

				//Link the textures to the pipeline.
				scene->LinkTexture(4, mView->GetCurrentTexture() - 1);
			}
		}
		else if (mView->GetCurrentRenderOption() == TEXTURES_PLUS_NO_SHADING)
		{
			scene->LinkPSOAndRootSignature(EARTH_MOON_SUN_NO_SHADING_PSO, EARTH_MOON_SUN_NO_SHADING_ROOT_SIG);
			//scene->LinkTexture(4, mView->GetCurrentTexture());

			if (mView->GetCurrentTexture() == EARTH)
			{
				//Link the textures to the pipeline.
				scene->LinkTexture(4, mView->GetCurrentTexture() + 2);
			}
			else //current texture is moon or sun
			{
				//Link the textures to the pipeline.
				scene->LinkTexture(4, mView->GetCurrentTexture() - 1);
			}
		}
		else if (mView->GetCurrentRenderOption() == COLOR_PLUS_SHADING)
		{
			scene->LinkPSOAndRootSignature(COLOR_SHADING_PSO, COLOR_SHADING_ROOT_SIG);
		}
		else if (mView->GetCurrentRenderOption() == COLOR_PLUS_NO_SHADING)
		{
			scene->LinkPSOAndRootSignature(COLOR_NO_SHADING_PSO, COLOR_SHADING_ROOT_SIG);
		}
		else if (mView->GetCurrentRenderOption() == WIREFRAME)
		{
			scene->LinkPSOAndRootSignature(WIREFRAME_PSO, COLOR_SHADING_ROOT_SIG);
		}

		//Link the vertex and index buffer to the pipeline
		scene->LinkStaticBuffer(FARender::VERTEX_BUFFER, SHAPES_VERTEX_BUFFER);
		scene->LinkStaticBuffer(FARender::INDEX_BUFFER, SHAPES_INDEX_BUFFER);

		//Link pass constant data to the pipeline
		scene->LinkDynamicBuffer(FARender::CONSTANT_BUFFER, PASSCB, 0, 1);

		scene->LinkDynamicBuffer(FARender::CONSTANT_BUFFER, MATERIALCB, 0, 2);

		scene->LinkDynamicBuffer(FARender::CONSTANT_BUFFER, LIGHTCB, 0, 3);

		mModel->GetShape(mView->GetCurrentShape())->RenderShape(scene);

		if (mView->GetCurrentRenderOption() == TEXTURES_PLUS_SHADING || mView->GetCurrentRenderOption() == COLOR_PLUS_SHADING)
		{
			scene->LinkPSOAndRootSignature(EARTH_MOON_SUN_NO_SHADING_PSO, EARTH_MOON_SUN_NO_SHADING_ROOT_SIG);

			scene->LinkTexture(4, 1);

			unsigned int numPointLightsToRender = mView->GetCurrentLightSource() + 1;
			for (unsigned int i = 0; i < numPointLightsToRender; ++i)
			{
				mModel->GetPointLight(i).GetShape().RenderShape(scene);
			}
		}

		//All the commands needed after rendering a shape
		scene->AfterRenderObjects();

		scene->AfterRender();
	}

	int Controller::Run()
	{
		MSG msg{};
		mModel->GetFrameTime().Reset();

		//Message Loop
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				mModel->GetFrameTime().Tick();

				if (mView->GetIsMainWindowActive())
				{
					FrameStats();
					UserInput();
					Update();
					Draw();
				}
			}
		}

		return (int)msg.wParam;
	}
}