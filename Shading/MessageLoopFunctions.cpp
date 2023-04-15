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
		//Poll keyboard and mouse input.
		camera.KeyboardInputWASD(frameTime.DeltaTime());
		camera.MouseInput();
	}

	void Update(FARender::RenderScene& scene)
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
		passConstantData.shadingType = currentSelection.at(SHADING) - 1; 

		//Copy the view and perspective projection matrices into the pass constant buffer
		scene.CopyDataIntoDynamicBuffer(PASSCB, 0, &passConstantData, sizeof(PassConstants));

		//Copy the view and perspective projection matrices into the pass constant buffer
		scene.CopyDataIntoDynamicBuffer(MATERIALCB, 0, &materials.at(currentSelection.at(MATERIALS) - 1), sizeof(Material));

		if (currentSelection.at(LIGHT_SOURCE) - 1 == POINT_LIGHT)
		{
			lightSources[0].lightSourceType = POINT_LIGHT;
			lightSources[1].lightSourceType = -1;
		}
		if (currentSelection.at(LIGHT_SOURCE) - 1 == DIRECTIONAL_LIGHT)
		{
			lightSources[0].lightSourceType = -1;
			lightSources[1].lightSourceType = DIRECTIONAL_LIGHT;
		}
		if (currentSelection.at(LIGHT_SOURCE) - 1 == POINT_PLUS_DIRECTIONAL_LIGHT)
		{
			lightSources[0].lightSourceType = POINT_LIGHT;
			lightSources[1].lightSourceType = DIRECTIONAL_LIGHT;
		}

		//Copy the view and perspective projection matrices into the pass constant buffer
		scene.CopyDataIntoDynamicBuffer(LIGHTCB, 0, lightSources.data(), lightSources.size() * sizeof(Light));

		static float angularVelocity{ 45.0f };

		for (auto& i : shapes)
		{
			//Rotate each shape around their local +y-axis.
			if (playAnimation)
			{
				i->RotateAxes(angularVelocity * frameTime.DeltaTime(), i->GetYAxis());
			}

			//Update each shapes local to world matrix
			i->UpdateLocalToWorldMatrix();

			ObjectConstants objectConstantData;
			objectConstantData.localToWorld = Transpose(i->GetLocalToWorldMatrix());

			//Don't transpose because hlsl will transpose when copying the data over.
			objectConstantData.inverseTransposeLocalToWorld = Inverse(i->GetLocalToWorldMatrix());

			//Copy the shapes local to world matrix into the object constant buffer.
			scene.CopyDataIntoDynamicBuffer(OBJECTCB, i->GetDrawArguments().indexOfConstantData,
				&objectConstantData, sizeof(ObjectConstants));
		}
	}

	void Draw(FARender::RenderScene& scene)
	{
		//All the commands needed before rendering the shapes.
		scene.BeforeRenderObjects(true);
		
		scene.SetPSOAndRootSignature(SHADING_PSO, 0);
	
		//Link the vertex and index buffer to the pipeline
		scene.SetStaticBuffer(0, VERTEX_BUFFER);
		scene.SetStaticBuffer(1, INDEX_BUFFER);

		//Link pass constant data to the pipeline
		scene.SetDynamicBuffer(2, PASSCB, 0, 1);
		
		//Link material constant data to the pipeline
		scene.SetDynamicBuffer(2, MATERIALCB, 0, 2);

		//Link light constant data to the pipeline
		scene.SetDynamicBuffer(2, LIGHTCB, 0, 3);

		//Get the draw arguments of the current shape.
		FAShapes::DrawArguments currentShapeDrawArguments{ shapes.at(currentSelection.at(SHAPES) - 1)->GetDrawArguments() };

		//Link the shapes constant data to the pipeline.
		scene.SetDynamicBuffer(2, OBJECTCB, currentShapeDrawArguments.indexOfConstantData, 0);

		//Render the current shape.
		scene.RenderObject(currentShapeDrawArguments.indexCount, currentShapeDrawArguments.locationOfFirstIndex,
			currentShapeDrawArguments.indexOfFirstVertex, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		//All the commands needed after rendering the shapes.
		scene.AfterRenderObjects(true, true);

		//All the commands needed before rendering text.
		scene.BeforeRenderText();

		//Render FPS
		scene.RenderText(framesPerSecond.GetTextLocation(), framesPerSecond.GetTextColor(), 
			framesPerSecond.GetTextSize(), framesPerSecond.GetTextString(), DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

		//Render selection arrow
		scene.RenderText(selectionArrow.GetTextLocation(), selectionArrow.GetTextColor(),
			selectionArrow.GetTextSize(), selectionArrow.GetTextString(), DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

		for (unsigned int i = 0; i <= numSelections; ++i)
		{
			FARender::Text textToRender(selections.at(i).at(0));
			scene.RenderText(textToRender.GetTextLocation(), textToRender.GetTextColor(),
				textToRender.GetTextSize(), textToRender.GetTextString(), DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

			textToRender = selections.at(i).at(currentSelection[i]);
			scene.RenderText(textToRender.GetTextLocation(), textToRender.GetTextColor(),
				textToRender.GetTextSize(), textToRender.GetTextString(), DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		}

		
		//All the commands needed after rendering text.
		scene.AfterRenderText();

		//All the commands needed after rendering the shapes and objects.
		scene.AfterRender();
	}
}