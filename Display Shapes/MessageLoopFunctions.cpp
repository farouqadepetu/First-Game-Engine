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
		if ((frameTime.TotalTime() - timeElapsed) >= 1.0f)
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
			timeElapsed += 1.0f;
		}
	}

	void UserInput()
	{
		//Poll keyboard and mouse input.
		camera.KeyboardInput(frameTime.DeltaTime());
		camera.MouseInput();
	}

	void Update(FARender::RenderScene& scene)
	{
		//Update the view matrix.
		camera.UpdateViewMatrix();

		//Update the perspective projection matrix.
		camera.UpdatePerspectiveProjectionMatrix();

		//Transpose and store the view and projection matrices in the PassConstants object.
		constantData.view = Transpose(camera.GetViewMatrix());
		constantData.projection = Transpose(camera.GetPerspectiveProjectionMatrix());

		//Copy the view and perspective projection matrices into the pass constant buffer
		scene.CopyDataIntoDynamicBuffer(PASSCB, 0, &constantData, sizeof(PassConstants));

		static float angularVelocity{ 45.0f };

		for (auto& i : shapes)
		{
			//Rotate each shape around their local y-axis.
			i->RotateAxes(angularVelocity * frameTime.DeltaTime(), i->GetYAxis());

			//Update each shapes local to world matrix
			i->UpdateLocalToWorldMatrix();

			//Copy the shapes local to world matrix into the object constant buffer.
			scene.CopyDataIntoDynamicBuffer(OBJECTCB, i->GetDrawArguments().indexOfConstantData,
				Transpose(i->GetLocalToWorldMatrix()).Data(), sizeof(FAMath::Matrix4x4));
		}
	}

	void Draw(FARender::RenderScene& scene)
	{
		//All the commands needed before rendering the shapes.
		scene.BeforeRenderObjects(isMSAAEnabled);

		if (isSolid && isMSAAEnabled)
		{
			//Set solid msaa pso
			scene.SetPSOAndRootSignature(SOLID_MSAA, 0);
		}
		else if (isSolid && !isMSAAEnabled)
		{
			//set solid pso
			scene.SetPSOAndRootSignature(SOLID, 0);
		}
		else if (!isSolid && isMSAAEnabled)
		{
			//set wire msaa pso
			scene.SetPSOAndRootSignature(WIRE_MSAA, 0);
		}
		else
		{
			//set wire pso
			scene.SetPSOAndRootSignature(WIRE, 0);
		}

		//Link the vertex and index buffer to the pipeline
		scene.SetStaticBuffer(0, VERTEX_BUFFER);
		scene.SetStaticBuffer(1, INDEX_BUFFER);

		//Link pass constant buffer to the pipeline
		scene.SetDynamicBuffer(2, PASSCB, 0, 1);

		for (auto& i : shapes)
		{
			FAShapes::DrawArguments shapeDrawArguments{ i->GetDrawArguments() };

			//Link the shapes constant data to the pipeline.
			scene.SetDynamicBuffer(2, OBJECTCB, shapeDrawArguments.indexOfConstantData, 0);

			//Render the shapes.
			scene.RenderObject(shapeDrawArguments.indexCount, shapeDrawArguments.locationOfFirstIndex,
				shapeDrawArguments.indexOfFirstVertex, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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
				scene.RenderText(i.GetTextLocation(), i.GetTextColor(), i.GetTextSize(), i.GetTextString());
			}

			//All the commands needed after rendering text.
			scene.AfterRenderText();
		}

		//All the commands needed after rendering the shapes and objects.
		scene.AfterRender();
	}
}