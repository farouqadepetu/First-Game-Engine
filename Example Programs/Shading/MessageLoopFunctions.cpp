#include "MessageLoopFunctions.h"
#include "DirectXException.h"
#include "GlobalVariables.h"
#include <iomanip>

using namespace GlobalVariables;

namespace MessageLoop
{
	void FrameStats()
	{
		//computes average frames per second and
		//the average time it takes to show each frame

		static unsigned int frameCount{ 0 };
		static float timeElapsed{ 0 };

		++frameCount;

		//after every second display fps and frame time.
		timeElapsed += frameTime.deltaTime;
		if (timeElapsed >= 1.0f)
		{
			float fps = (float)frameCount;
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
			framesPerSecond.SetTextString(textStr);

			//reset for next average
			frameCount = 0;
			timeElapsed = 0.0f;
		}
	}

	void UserInput()
	{
		if (enableCameraUserInput)
		{
			//Poll keyboard and mouse input.
			//check if w, a, s, d or up, left, right, down, spacebar or control was pressed

			if (GetAsyncKeyState('W') & 0x8000 || GetAsyncKeyState(VK_UP) & 0x8000)
				RenderingEngine::Foward(camera, frameTime.deltaTime);
			if (GetAsyncKeyState('A') & 0x8000 || GetAsyncKeyState(VK_LEFT) & 0x8000)
				RenderingEngine::Left(camera, frameTime.deltaTime);
			if (GetAsyncKeyState('S') & 0x8000 || GetAsyncKeyState(VK_DOWN) & 0x8000)
				RenderingEngine::Backward(camera, frameTime.deltaTime);
			if (GetAsyncKeyState('D') & 0x8000 || GetAsyncKeyState(VK_RIGHT) & 0x8000)
				RenderingEngine::Right(camera, frameTime.deltaTime);
			if (GetAsyncKeyState(VK_SPACE) & 0x8000)
				RenderingEngine::Up(camera, frameTime.deltaTime);
			if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
				RenderingEngine::Down(camera, frameTime.deltaTime);

			POINT currMousePos{};
			GetCursorPos(&currMousePos);

			vec2 currentMousePosition{ (float)currMousePos.x, (float)currMousePos.y };

			vec2 mousePositionDiff{ currentMousePosition - lastMousePosition };

			//if the mouse goes outside the window and comes back into the window, the camera won't be rotated.
			if (Length(mousePositionDiff) < 10.0f && (GetAsyncKeyState(VK_LBUTTON) & 0x8000))
			{
				RenderingEngine::RotateCameraLeftRight(camera, camera.angularSpeed * mousePositionDiff.x);
				RenderingEngine::RotateCameraUpDown(camera, camera.angularSpeed * mousePositionDiff.y);
			}

			lastMousePosition = currentMousePosition;
		}
	}

	void Update()
	{
		//Update the view matrix.
		UpdateViewMatrix(camera);

		//Update the perspective projection matrix.
		UpdateProjectionMatrix(pProjection);

		//Copy the pass data into the pass constant buffer.
		PassConstantBuffer passConstantData;
		passConstantData.passConstants.view = Transpose(camera.viewMatrix);
		passConstantData.passConstants.projection = Transpose(pProjection.projectionMatrix);
		passConstantData.passConstants.cameraPosition = camera.position;
		passConstantData.passConstants.shadingType = currentSelection.at(SHADING);
		shadingScene->CopyDataIntoDynamicBuffer(PASSCB, 0, &passConstantData, sizeof(PassConstantBuffer));

		//Copy the material data into the material constant buffer.
		MaterialConstantBuffer materialCB;
		materialCB.material = materials.at(currentSelection.at(MATERIALS));
		shadingScene->CopyDataIntoDynamicBuffer(MATERIALCB, 0, &materialCB, sizeof(MaterialConstantBuffer));

		if (currentSelection.at(LIGHT_SOURCE) == POINT_LIGHT)
		{
			lightSources[0].lightSourceType = POINT_LIGHT;
			lightSources[1].lightSourceType = -1;
		}
		if (currentSelection.at(LIGHT_SOURCE) == DIRECTIONAL_LIGHT)
		{
			lightSources[0].lightSourceType = -1;
			lightSources[1].lightSourceType = DIRECTIONAL_LIGHT;
		}
		if (currentSelection.at(LIGHT_SOURCE) == POINT_PLUS_DIRECTIONAL_LIGHT)
		{
			lightSources[0].lightSourceType = POINT_LIGHT;
			lightSources[1].lightSourceType = DIRECTIONAL_LIGHT;
		}


		//Copy the light data into the light constant buffer.
		LightConstantBuffer lights;
		for (unsigned int i = 0; i < MAX_NUM_LIGHTS; ++i)
		{
			lights.lightSources[i] = lightSources[i];
		}
		shadingScene->CopyDataIntoDynamicBuffer(LIGHTCB, 0, &lights, sizeof(LightConstantBuffer));

		static float angularVelocity{ 45.0f };
		
		//Rotate each shape around their local +y-axis.
		if (playAnimation)
		{
			switch (currentSelection.at(SHAPES))
			{
				case BOX:
				{
					box.SetOrientation(MathEngine::Normalize(
						MathEngine::RotationQuaternion(angularVelocity * frameTime.deltaTime, vec3{ 0.0f, 1.0f, 0.0f }) *
						box.GetOrientation()));

					break;
				}

				case PYRAMID:
				{
					pyramid.SetOrientation(MathEngine::Normalize(
						MathEngine::RotationQuaternion(angularVelocity * frameTime.deltaTime, vec3{ 0.0f, 1.0f, 0.0f }) *
						pyramid.GetOrientation()));

					break;
				}

				case SPHERE:
				{
					sphere.SetOrientation(MathEngine::Normalize(
						MathEngine::RotationQuaternion(angularVelocity * frameTime.deltaTime, vec3{ 0.0f, 1.0f, 0.0f }) *
						sphere.GetOrientation()));

					break;
				}

				case CYLINDER:
				{
					cylinder.SetOrientation(MathEngine::Normalize(
						MathEngine::RotationQuaternion(angularVelocity * frameTime.deltaTime, vec3{ 0.0f, 1.0f, 0.0f }) *
						cylinder.GetOrientation()));

					break;
				}

				case CONE:
				{
					cone.SetOrientation(MathEngine::Normalize(
						MathEngine::RotationQuaternion(angularVelocity * frameTime.deltaTime, vec3{ 0.0f, 1.0f, 0.0f }) *
						cone.GetOrientation()));

					break;
				}
			}
		}

		//Update each shapes local to world matrix
		box.UpdateModelMatrix();
		pyramid.UpdateModelMatrix();
		sphere.UpdateModelMatrix();
		cylinder.UpdateModelMatrix();
		cone.UpdateModelMatrix();

		ObjectConstantBuffer objectConstantData;
		switch (currentSelection.at(SHAPES))
		{
			case BOX:
			{
				objectConstantData.objectConstants.localToWorld = Transpose(box.GetModelMatrix());

				//Don't transpose because hlsl will transpose when copying the data over.
				objectConstantData.objectConstants.inverseTransposeLocalToWorld =
					Inverse(box.GetModelMatrix());

				//Copy the shapes local to world matrix into the object constant buffer.
				RenderingEngine::Update(shadingScene.get(), box.GetDrawArguments(), &objectConstantData, sizeof(ObjectConstantBuffer));

				break;
			}

			case PYRAMID:
			{
				objectConstantData.objectConstants.localToWorld = Transpose(pyramid.GetModelMatrix());

				//Don't transpose because hlsl will transpose when copying the data over.
				objectConstantData.objectConstants.inverseTransposeLocalToWorld =
					Inverse(pyramid.GetModelMatrix());

				//Copy the shapes local to world matrix into the object constant buffer.
				RenderingEngine::Update(shadingScene.get(), pyramid.GetDrawArguments(), &objectConstantData, sizeof(ObjectConstantBuffer));

				break;
			}

			case SPHERE:
			{
				objectConstantData.objectConstants.localToWorld = Transpose(sphere.GetModelMatrix());

				//Don't transpose because hlsl will transpose when copying the data over.
				objectConstantData.objectConstants.inverseTransposeLocalToWorld =
					Inverse(sphere.GetModelMatrix());

				//Copy the shapes local to world matrix into the object constant buffer.
				RenderingEngine::Update(shadingScene.get(), sphere.GetDrawArguments(), &objectConstantData, sizeof(ObjectConstantBuffer));

				break;
			}

			case CYLINDER:
			{
				objectConstantData.objectConstants.localToWorld = Transpose(cylinder.GetModelMatrix());

				//Don't transpose because hlsl will transpose when copying the data over.
				objectConstantData.objectConstants.inverseTransposeLocalToWorld =
					Inverse(cylinder.GetModelMatrix());

				//Copy the shapes local to world matrix into the object constant buffer.
				RenderingEngine::Update(shadingScene.get(), cylinder.GetDrawArguments(), &objectConstantData, sizeof(ObjectConstantBuffer));

				break;
			}

			case CONE:
			{
				objectConstantData.objectConstants.localToWorld = Transpose(cone.GetModelMatrix());

				//Don't transpose because hlsl will transpose when copying the data over.
				objectConstantData.objectConstants.inverseTransposeLocalToWorld =
					Inverse(cone.GetModelMatrix());

				//Copy the shapes local to world matrix into the object constant buffer.
				RenderingEngine::Update(shadingScene.get(), cone.GetDrawArguments(), &objectConstantData, sizeof(ObjectConstantBuffer));

				break;
			}
		}
	}

	void Draw()
	{
		//All the commands needed before rendering the shapes.
		shadingScene->BeforeRenderObjects(true);
		
		shadingScene->LinkPSOAndRootSignature(SHADING_PSO, 0);
	
		//Link the vertex and index buffer to the pipeline
		shadingScene->LinkStaticBuffer(RenderingEngine::VERTEX_BUFFER, SHAPES_VERTEX_BUFFER);
		shadingScene->LinkStaticBuffer(RenderingEngine::INDEX_BUFFER, SHAPES_INDEX_BUFFER);

		//Link pass constant data to the pipeline
		shadingScene->LinkDynamicBuffer(RenderingEngine::CONSTANT_BUFFER, PASSCB, 0, 1);
		
		//Link material constant data to the pipeline
		shadingScene->LinkDynamicBuffer(RenderingEngine::CONSTANT_BUFFER, MATERIALCB, 0, 2);

		//Link light constant data to the pipeline
		shadingScene->LinkDynamicBuffer(RenderingEngine::CONSTANT_BUFFER, LIGHTCB, 0, 3);

		switch (currentSelection.at(SHAPES))
		{
			case BOX:
			{
				RenderingEngine::Render(shadingScene.get(), box.GetDrawArguments());

				break;
			}

			case PYRAMID:
			{
				RenderingEngine::Render(shadingScene.get(), pyramid.GetDrawArguments());

				break;
			}

			case SPHERE:
			{
				RenderingEngine::Render(shadingScene.get(), sphere.GetDrawArguments());

				break;
			}

			case CYLINDER:
			{
				RenderingEngine::Render(shadingScene.get(), cylinder.GetDrawArguments());

				break;
			}

			case CONE:
			{
				RenderingEngine::Render(shadingScene.get(), cone.GetDrawArguments());

				break;
			}
		}
		
		//All the commands needed after rendering the shapes.
		shadingScene->AfterRenderObjects(true, true);

		//All the commands needed before rendering text.
		shadingScene->BeforeRenderText();

		//Render FPS
		shadingScene->RenderText(framesPerSecond.GetTextLocation(), framesPerSecond.GetTextColor(),
			framesPerSecond.GetTextSize(), framesPerSecond.GetTextString(), DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

		//All the commands needed after rendering text.
		shadingScene->AfterRenderText();

		//All the commands needed after rendering the shapes and objects.
		shadingScene->AfterRender();
	}
}