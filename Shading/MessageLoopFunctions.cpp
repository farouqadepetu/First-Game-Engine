#include "MessageLoopFunctions.h"
#include "FADirectXException.h"
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
			framesPerSecond.SetTextString(textStr);

			//reset for next average
			frameCount = 0;
			timeElapsed += 1.0f;
		}
	}

	void UserInput()
	{
		if (enableCameraUserInput)
		{
			//Poll keyboard and mouse input.
			camera.KeyboardInputWASD(frameTime.DeltaTime());
			camera.MouseInput();
		}
	}

	void Update()
	{
		//Update the view matrix.
		camera.UpdateViewMatrix();

		//Update the perspective projection matrix.
		camera.UpdatePerspectiveProjectionMatrix();

		PassConstants passConstantData;

		//Transpose and store the view and projection matrices in the PassConstants object.
		passConstantData.view = Transpose(camera.GetViewMatrix());
		passConstantData.projection = Transpose(camera.GetPerspectiveProjectionMatrix());
		passConstantData.cameraPosition = camera.GetCameraPosition();
		passConstantData.shadingType = currentSelection.at(SHADING); 

		//Copy the view and perspective projection matrices into the pass constant buffer
		shadingScene->CopyDataIntoDynamicBuffer(PASSCB, 0, &passConstantData, sizeof(PassConstants));

		//Copy the view and perspective projection matrices into the pass constant buffer
		shadingScene->CopyDataIntoDynamicBuffer(MATERIALCB, 0, &materials.at(currentSelection.at(MATERIALS)), sizeof(Material));

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

		//Copy the view and perspective projection matrices into the pass constant buffer
		shadingScene->CopyDataIntoDynamicBuffer(LIGHTCB, 0, lightSources.data(), lightSources.size() * sizeof(Light));

		static float angularVelocity{ 45.0f };
		
		//Rotate each shape around their local +y-axis.
		if (playAnimation)
		{
			shapes.at(currentSelection.at(SHAPES))->RotateAxes(angularVelocity * frameTime.DeltaTime(), 
				shapes.at(currentSelection.at(SHAPES))->GetYAxis());
		}

		//Update each shapes local to world matrix
		shapes.at(currentSelection.at(SHAPES))->UpdateLocalToWorldMatrix();

		ObjectConstants objectConstantData;
		objectConstantData.localToWorld = Transpose(shapes.at(currentSelection.at(SHAPES))->GetLocalToWorldMatrix());

		//Don't transpose because hlsl will transpose when copying the data over.
		objectConstantData.inverseTransposeLocalToWorld = Inverse(shapes.at(currentSelection.at(SHAPES))->GetLocalToWorldMatrix());

		//Copy the shapes local to world matrix into the object constant buffer.
		shadingScene->CopyDataIntoDynamicBuffer(OBJECTCB, shapes.at(currentSelection.at(SHAPES))->GetDrawArguments().indexOfConstantData,
			&objectConstantData, sizeof(ObjectConstants));
	}

	void Draw()
	{
		//All the commands needed before rendering the shapes.
		shadingScene->BeforeRenderObjects(true);
		
		shadingScene->SetPSOAndRootSignature(SHADING_PSO, 0);
	
		//Link the vertex and index buffer to the pipeline
		shadingScene->SetStaticBuffer(FARender::VERTEX_BUFFER, SHAPES_VERTEX_BUFFER);
		shadingScene->SetStaticBuffer(FARender::INDEX_BUFFER, SHAPES_INDEX_BUFFER);

		//Link pass constant data to the pipeline
		shadingScene->SetDynamicBuffer(FARender::CONSTANT_BUFFER, PASSCB, 0, 1);
		
		//Link material constant data to the pipeline
		shadingScene->SetDynamicBuffer(FARender::CONSTANT_BUFFER, MATERIALCB, 0, 2);

		//Link light constant data to the pipeline
		shadingScene->SetDynamicBuffer(FARender::CONSTANT_BUFFER, LIGHTCB, 0, 3);

		//Get the draw arguments of the current shape.
		FAShapes::DrawArguments currentShapeDrawArguments{ shapes.at(currentSelection.at(SHAPES))->GetDrawArguments() };

		//Link the shapes constant data to the pipeline.
		shadingScene->SetDynamicBuffer(FARender::CONSTANT_BUFFER, OBJECTCB, currentShapeDrawArguments.indexOfConstantData, 0);

		//Render the current shape.
		shadingScene->RenderObject(currentShapeDrawArguments.indexCount, currentShapeDrawArguments.locationOfFirstIndex,
			currentShapeDrawArguments.indexOfFirstVertex, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
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