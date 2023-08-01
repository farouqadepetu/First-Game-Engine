#include "MessageLoopFunctions.h"
#include "DirectXException.h"
#include "DisplayShapesGlobalVariables.h"
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
			textList.at(FRAMES_PER_SECOND).SetTextString(textStr);

			//reset for next average
			frameCount = 0;
			timeElapsed = 0.0f;
		}
	}

	void UserInput()
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

	void Update(RenderingEngine::RenderScene& scene)
	{
		//Update the view matrix.
		RenderingEngine::UpdateViewMatrix(camera);

		//Update the perspective projection matrix.
		RenderingEngine::UpdateProjectionMatrix(pProjection);

		//Transpose and store the view and projection matrices in the PassConstants object.
		constantData.view = Transpose(camera.viewMatrix);
		constantData.projection = Transpose(pProjection.projectionMatrix);

		//Copy the view and perspective projection matrices into the pass constant buffer
		scene.CopyDataIntoDynamicBuffer(L"PASSCB", 0, &constantData, sizeof(PassConstants));

		static float angularSpeed{ 45.0f };
		MathEngine::Quaternion rotate(MathEngine::RotationQuaternion(angularSpeed * frameTime.deltaTime, vec3{ 0.0f, 1.0f, 0.0f }));

		box.SetOrientation(MathEngine::Normalize(rotate * box.GetOrientation()));
		pyramid.SetOrientation(MathEngine::Normalize(rotate * box.GetOrientation()));
		sphere.SetOrientation(MathEngine::Normalize(rotate * box.GetOrientation()));
		cylinder.SetOrientation(MathEngine::Normalize(rotate * box.GetOrientation()));
		cone.SetOrientation(MathEngine::Normalize(rotate * box.GetOrientation()));

		box.UpdateModelMatrix();
		pyramid.UpdateModelMatrix();
		sphere.UpdateModelMatrix();
		cylinder.UpdateModelMatrix();
		cone.UpdateModelMatrix();

		ObjectConstants ob;
		ob.MVP = Transpose(box.GetModelMatrix());
		ob.color = box.GetColor();
		RenderingEngine::Update(&scene, box.GetDrawArguments(), &ob, sizeof(ObjectConstants));

		ob.MVP = Transpose(pyramid.GetModelMatrix());
		ob.color = pyramid.GetColor();
		RenderingEngine::Update(&scene, pyramid.GetDrawArguments(), &ob, sizeof(ObjectConstants));

		ob.MVP = Transpose(sphere.GetModelMatrix());
		ob.color = sphere.GetColor();
		RenderingEngine::Update(&scene, sphere.GetDrawArguments(), &ob, sizeof(ObjectConstants));

		ob.MVP = Transpose(cylinder.GetModelMatrix());
		ob.color = cylinder.GetColor();
		RenderingEngine::Update(&scene, cylinder.GetDrawArguments(), &ob, sizeof(ObjectConstants));

		ob.MVP = Transpose(cone.GetModelMatrix());
		ob.color = cone.GetColor();
		RenderingEngine::Update(&scene, cone.GetDrawArguments(), &ob, sizeof(ObjectConstants));

	}

	void Draw(RenderingEngine::RenderScene& scene)
	{
		//All the commands needed before rendering the shapes.
		scene.BeforeRenderObjects(isMSAAEnabled);

		if (isSolid && isMSAAEnabled)
		{
			//Set solid msaa pso
			scene.LinkPSOAndRootSignature(SOLID_MSAA, 0);
		}
		else if (isSolid && !isMSAAEnabled)
		{
			//set solid pso
			scene.LinkPSOAndRootSignature(SOLID, 0);
		}
		else if (!isSolid && isMSAAEnabled)
		{
			//set wire msaa pso
			scene.LinkPSOAndRootSignature(WIRE_MSAA, 0);
		}
		else
		{
			//set wire pso
			scene.LinkPSOAndRootSignature(WIRE, 0);
		}

		//Link the vertex and index buffer to the pipeline
		scene.LinkStaticBuffer(0, L"Vertex Buffer");
		scene.LinkStaticBuffer(1, L"Index Buffer");

		//Link pass constant buffer to the pipeline
		scene.LinkDynamicBuffer(2, L"PASSCB", 0, 1);

		//Render all the shapes
		for (const auto& i : shapes)
		{
			RenderingEngine::Render(&scene, i);
		}

		//All the commands needed after rendering the shapes.
		scene.AfterRenderObjects(isMSAAEnabled, isTextEnabled);

		//If text is enabled.
		if (isTextEnabled)
		{
			//All the commands needed before rendering text.
			scene.BeforeRenderText();

			for (auto& i : textList)
			{
				scene.RenderText(i.GetTextLocation(), i.GetTextColor(), i.GetTextSize(), i.GetTextString(), DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
			}

			//All the commands needed after rendering text.
			scene.AfterRenderText();
		}

		//All the commands needed after rendering the shapes and objects.
		scene.AfterRender();
	}
}