#include "Controller.h"
#include "DirectXException.h"
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

		timeElapsed += mModel->GetFrameTime().deltaTime;
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
			SetWindowText(mView->GetMainWindow().windowHandle, textStr.c_str());

			//reset for next average
			frameCount = 0;
			timeElapsed = 0.0f;
		}
	}

	void Controller::UserInput()
	{
		if (mView->GetCameraMovement())
		{
			//Poll keyboard and mouse input.
			//check if w, a, s, d or up, left, right, down, spacebar or control was pressed

			if (GetAsyncKeyState('W') & 0x8000 || GetAsyncKeyState(VK_UP) & 0x8000)
				RenderingEngine::Foward(mModel->GetCamera(), mModel->GetFrameTime().deltaTime);
			if (GetAsyncKeyState('A') & 0x8000 || GetAsyncKeyState(VK_LEFT) & 0x8000)
				RenderingEngine::Left(mModel->GetCamera(), mModel->GetFrameTime().deltaTime);
			if (GetAsyncKeyState('S') & 0x8000 || GetAsyncKeyState(VK_DOWN) & 0x8000)
				RenderingEngine::Backward(mModel->GetCamera(), mModel->GetFrameTime().deltaTime);
			if (GetAsyncKeyState('D') & 0x8000 || GetAsyncKeyState(VK_RIGHT) & 0x8000)
				RenderingEngine::Right(mModel->GetCamera(), mModel->GetFrameTime().deltaTime);
			if (GetAsyncKeyState(VK_SPACE) & 0x8000)
				RenderingEngine::Up(mModel->GetCamera(), mModel->GetFrameTime().deltaTime);
			if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
				RenderingEngine::Down(mModel->GetCamera(), mModel->GetFrameTime().deltaTime);

			POINT currMousePos{};
			GetCursorPos(&currMousePos);

			vec2 currentMousePosition{ (float)currMousePos.x, (float)currMousePos.y };

			vec2 mousePositionDiff{ currentMousePosition - mLastMousePosition };

			//if the mouse goes outside the window and comes back into the window, the camera won't be rotated.
			if (Length(mousePositionDiff) < 10.0f && (GetAsyncKeyState(VK_LBUTTON) & 0x8000))
			{
				RenderingEngine::RotateCameraLeftRight(mModel->GetCamera(), mModel->GetCamera().angularSpeed * mousePositionDiff.x);
				RenderingEngine::RotateCameraUpDown(mModel->GetCamera(), mModel->GetCamera().angularSpeed * mousePositionDiff.y);
			}

			mLastMousePosition = currentMousePosition;
		}
	}

	void Controller::Update()
	{
		RenderingEngine::UpdateViewMatrix(mModel->GetCamera());
		RenderingEngine::UpdateProjectionMatrix(mModel->GetPerspectiveProjection());

		PassConstantBuffer passConstantsData;
		passConstantsData.passConstants.view = Transpose(mModel->GetCamera().viewMatrix);
		passConstantsData.passConstants.projection = Transpose(mModel->GetPerspectiveProjection().projectionMatrix);
		passConstantsData.passConstants.cameraPosition = mModel->GetCamera().position;

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
			lights.lightSources[i].color = RenderingEngine::Color(0.0f, 0.0f, 0.0f, 1.0f);
		}

		mModel->GetScene()->CopyDataIntoDynamicBuffer(LIGHTCB, 0, &lights, sizeof(LightConstantBuffer));

		if (mView->GetRotateShape())
		{
			switch (mView->GetCurrentShape())
			{
				case BOX:
				{
					mModel->box.SetOrientation(MathEngine::Normalize(
						MathEngine::RotationQuaternion(angularVelocity * mModel->GetFrameTime().deltaTime, vec3{ 0.0f, 1.0f, 0.0f }) *
						mModel->box.GetOrientation()));

					break;
				}

				case PYRAMID:
				{
					mModel->pyramid.SetOrientation(MathEngine::Normalize(
						MathEngine::RotationQuaternion(angularVelocity * mModel->GetFrameTime().deltaTime, vec3{ 0.0f, 1.0f, 0.0f }) *
						mModel->pyramid.GetOrientation()));

					break;
				}

				case SPHERE:
				{
					mModel->sphere.SetOrientation(MathEngine::Normalize(
						MathEngine::RotationQuaternion(angularVelocity * mModel->GetFrameTime().deltaTime, vec3{ 0.0f, 1.0f, 0.0f }) *
						mModel->sphere.GetOrientation()));

					break;
				}

				case CYLINDER:
				{
					mModel->cylinder.SetOrientation(MathEngine::Normalize(
						MathEngine::RotationQuaternion(angularVelocity * mModel->GetFrameTime().deltaTime, vec3{ 0.0f, 1.0f, 0.0f }) *
						mModel->cylinder.GetOrientation()));

					break;
				}

				case CONE:
				{
					mModel->cone.SetOrientation(MathEngine::Normalize(
						MathEngine::RotationQuaternion(angularVelocity * mModel->GetFrameTime().deltaTime, vec3{ 0.0f, 1.0f, 0.0f }) *
						mModel->cone.GetOrientation()));

					break;
				}
			}
		}

		mModel->box.UpdateModelMatrix();
		mModel->pyramid.UpdateModelMatrix();
		mModel->sphere.UpdateModelMatrix();
		mModel->cylinder.UpdateModelMatrix();
		mModel->cone.UpdateModelMatrix();

		ObjectConstantBuffer objectConstantData;
		switch (mView->GetCurrentShape())
		{
			case BOX:
			{
				objectConstantData.objectConstants.localToWorld = Transpose(mModel->box.GetModelMatrix());

				//Don't transpose because hlsl will transpose when copying the data over.
				objectConstantData.objectConstants.inverseTransposeLocalToWorld =
					Inverse(mModel->box.GetModelMatrix());

				objectConstantData.objectConstants.color = mModel->box.GetColor();

				//Copy the shapes local to world matrix into the object constant buffer.
				RenderingEngine::Update(mModel->GetScene(), mModel->box.GetDrawArguments(), &objectConstantData, sizeof(ObjectConstantBuffer));

				break;
			}

			case PYRAMID:
			{
				objectConstantData.objectConstants.localToWorld = Transpose(mModel->pyramid.GetModelMatrix());

				//Don't transpose because hlsl will transpose when copying the data over.
				objectConstantData.objectConstants.inverseTransposeLocalToWorld =
					Inverse(mModel->pyramid.GetModelMatrix());

				objectConstantData.objectConstants.color = mModel->pyramid.GetColor();

				//Copy the shapes local to world matrix into the object constant buffer.
				RenderingEngine::Update(mModel->GetScene(), mModel->pyramid.GetDrawArguments(), &objectConstantData, sizeof(ObjectConstantBuffer));

				break;
			}

			case SPHERE:
			{
				objectConstantData.objectConstants.localToWorld = Transpose(mModel->sphere.GetModelMatrix());

				//Don't transpose because hlsl will transpose when copying the data over.
				objectConstantData.objectConstants.inverseTransposeLocalToWorld =
					Inverse(mModel->sphere.GetModelMatrix());

				objectConstantData.objectConstants.color = mModel->sphere.GetColor();

				//Copy the shapes local to world matrix into the object constant buffer.
				RenderingEngine::Update(mModel->GetScene(), mModel->sphere.GetDrawArguments(), &objectConstantData, sizeof(ObjectConstantBuffer));

				break;
			}

			case CYLINDER:
			{
				objectConstantData.objectConstants.localToWorld = Transpose(mModel->cylinder.GetModelMatrix());

				//Don't transpose because hlsl will transpose when copying the data over.
				objectConstantData.objectConstants.inverseTransposeLocalToWorld =
					Inverse(mModel->cylinder.GetModelMatrix());

				objectConstantData.objectConstants.color = mModel->cylinder.GetColor();

				//Copy the shapes local to world matrix into the object constant buffer.
				RenderingEngine::Update(mModel->GetScene(), mModel->cylinder.GetDrawArguments(), &objectConstantData, sizeof(ObjectConstantBuffer));

				break;
			}

			case CONE:
			{
				objectConstantData.objectConstants.localToWorld = Transpose(mModel->cone.GetModelMatrix());

				//Don't transpose because hlsl will transpose when copying the data over.
				objectConstantData.objectConstants.inverseTransposeLocalToWorld =
					Inverse(mModel->cone.GetModelMatrix());

				objectConstantData.objectConstants.color = mModel->cone.GetColor();

				//Copy the shapes local to world matrix into the object constant buffer.
				RenderingEngine::Update(mModel->GetScene(), mModel->cone.GetDrawArguments(), &objectConstantData, sizeof(ObjectConstantBuffer));

				break;
			}
		}


		for (unsigned int i = 0; i < MAX_NUM_LIGHTS; ++i)
		{
			if (mView->GetRotatePointLight())
			{
				if (i == 0 || i == 1)
				{
					mModel->GetLightSources().at(i).position = 
						MathEngine::Rotate(MathEngine::RotationQuaternion(angularVelocity * mModel->GetFrameTime().deltaTime, 0.0f, 1.0f, 0.0f),
						mModel->GetLightSources().at(i).position);

					mModel->GetPointLight(i).SetPosition(mModel->GetLightSources().at(i).position);
				}
				else
				{
					mModel->GetLightSources().at(i).position =
						MathEngine::Rotate(MathEngine::RotationQuaternion(angularVelocity * mModel->GetFrameTime().deltaTime, 1.0f, 0.0f, 0.0f),
							mModel->GetLightSources().at(i).position);

					mModel->GetPointLight(i).SetPosition(mModel->GetLightSources().at(i).position);
				}
			}

			mModel->GetPointLight(i).UpdateModelMatrix();

			ObjectConstantBuffer pointLightConstantData;
			MathEngine::Matrix4x4 modelTrans(mModel->GetPointLight(i).GetModelMatrix());
			pointLightConstantData.objectConstants.localToWorld = Transpose(modelTrans);
			pointLightConstantData.objectConstants.inverseTransposeLocalToWorld = Inverse(modelTrans);
			pointLightConstantData.objectConstants.color = mModel->GetPointLight(i).GetColor();

			RenderingEngine::Update(mModel->GetScene(), mModel->GetPointLight(i).GetDrawArguments(), &pointLightConstantData, sizeof(ObjectConstantBuffer));
		}
	}

	void Controller::Draw()
	{
		RenderingEngine::RenderScene* scene{ mModel->GetScene() };

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
		scene->LinkStaticBuffer(RenderingEngine::VERTEX_BUFFER, SHAPES_VERTEX_BUFFER);
		scene->LinkStaticBuffer(RenderingEngine::INDEX_BUFFER, SHAPES_INDEX_BUFFER);

		//Link pass constant data to the pipeline
		scene->LinkDynamicBuffer(RenderingEngine::CONSTANT_BUFFER, PASSCB, 0, 1);

		scene->LinkDynamicBuffer(RenderingEngine::CONSTANT_BUFFER, MATERIALCB, 0, 2);

		scene->LinkDynamicBuffer(RenderingEngine::CONSTANT_BUFFER, LIGHTCB, 0, 3);

		switch (mView->GetCurrentShape())
		{
			case BOX:
			{
				RenderingEngine::Render(mModel->GetScene(), mModel->box.GetDrawArguments());

				break;
			}

			case PYRAMID:
			{
				RenderingEngine::Render(mModel->GetScene(), mModel->pyramid.GetDrawArguments());

				break;
			}

			case SPHERE:
			{
				RenderingEngine::Render(mModel->GetScene(), mModel->sphere.GetDrawArguments());

				break;
			}

			case CYLINDER:
			{
				RenderingEngine::Render(mModel->GetScene(), mModel->cylinder.GetDrawArguments());

				break;
			}

			case CONE:
			{
				RenderingEngine::Render(mModel->GetScene(), mModel->cone.GetDrawArguments());

				break;
			}
		}

		if (mView->GetCurrentRenderOption() == TEXTURES_PLUS_SHADING || mView->GetCurrentRenderOption() == COLOR_PLUS_SHADING)
		{
			scene->LinkPSOAndRootSignature(EARTH_MOON_SUN_NO_SHADING_PSO, EARTH_MOON_SUN_NO_SHADING_ROOT_SIG);

			scene->LinkTexture(4, 1);

			unsigned int numPointLightsToRender = mView->GetCurrentLightSource() + 1;
			for (unsigned int i = 0; i < numPointLightsToRender; ++i)
			{
				RenderingEngine::Render(mModel->GetScene(), mModel->GetPointLight(i).GetDrawArguments());
			}
		}

		//All the commands needed after rendering a shape
		scene->AfterRenderObjects();

		scene->AfterRender();
	}

	int Controller::Run()
	{
		MSG msg{};

		RenderingEngine::InitializeTime(mModel->GetFrameTime());
		RenderingEngine::Reset(mModel->GetFrameTime());

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
				RenderingEngine::Tick(mModel->GetFrameTime());

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