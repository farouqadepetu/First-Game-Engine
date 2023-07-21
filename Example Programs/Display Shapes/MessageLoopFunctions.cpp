#include "MessageLoopFunctions.h"
#include "FADirectXException.h"
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
		timeElapsed += frameTime.GetDeltaTime();
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
		camera.KeyboardInput(frameTime.GetDeltaTime());
		camera.MouseInput();
	}

	void Update(FARender::RenderScene& scene)
	{
		//Update the view matrix.
		camera.UpdateViewMatrix();

		//Update the perspective projection matrix.
		pProjection.UpdateProjectionMatrix();

		//Transpose and store the view and projection matrices in the PassConstants object.
		constantData.view = Transpose(camera.GetViewMatrix());
		constantData.projection = Transpose(pProjection.GetProjectionMatrix());

		//Copy the view and perspective projection matrices into the pass constant buffer
		scene.CopyDataIntoDynamicBuffer(L"PASSCB", 0, &constantData, sizeof(PassConstants));

		static float angularVelocity{ 45.0f };
		FAMath::Quaternion rotate(FAMath::RotationQuaternion(angularVelocity * frameTime.GetDeltaTime(), FAMath::Vector3D(0.0f, 1.0f, 0.0f)));

		for (auto& i : shapes)
		{
			i->SetOrientation(FAMath::Normalize(rotate * i->GetOrientation()));
		}

		box.UpdateModelMatrix();
		pyramid.UpdateModelMatrix();
		sphere.UpdateModelMatrix();
		cylinder.UpdateModelMatrix();
		cone.UpdateModelMatrix();

		ObjectConstants ob;
		for (auto& i : shapes)
		{
			ob.MVP = Transpose(i->GetModelMatrix());
			ob.color = i->GetColor();
			i->UpdateShape(&scene, &ob, sizeof(ObjectConstants));
		}
	}

	void Draw(FARender::RenderScene& scene)
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

		for (const auto& i : shapes)
		{
			i->RenderShape(&scene);
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