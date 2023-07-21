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
			camera.KeyboardInputWASD(frameTime.GetDeltaTime());
			camera.MouseInput();
		}
	}

	void Update()
	{
		//Update the view matrix.
		camera.UpdateViewMatrix();

		//Update the perspective projection matrix.
		pProjection.UpdateProjectionMatrix();

		//Copy the pass data into the pass constant buffer.
		PassConstantBuffer passConstantData;
		passConstantData.passConstants.view = Transpose(camera.GetViewMatrix());
		passConstantData.passConstants.projection = Transpose(pProjection.GetProjectionMatrix());
		passConstantData.passConstants.cameraPosition = camera.GetCameraPosition();
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
			shapes.at(currentSelection.at(SHAPES))->SetOrientation(Normalize(
				FAMath::RotationQuaternion(angularVelocity * frameTime.GetDeltaTime(), FAMath::Vector3D(0.0f, 1.0f, 0.0f)) *
				shapes.at(currentSelection.at(SHAPES))->GetOrientation()));
		}

		//Update each shapes local to world matrix
		box.UpdateModelMatrix();
		pyramid.UpdateModelMatrix();
		sphere.UpdateModelMatrix();
		cylinder.UpdateModelMatrix();
		cone.UpdateModelMatrix();

		ObjectConstantBuffer objectConstantData;
		objectConstantData.objectConstants.localToWorld = Transpose(shapes.at(currentSelection.at(SHAPES))->GetModelMatrix());

		//Don't transpose because hlsl will transpose when copying the data over.
		objectConstantData.objectConstants.inverseTransposeLocalToWorld = 
			Inverse(shapes.at(currentSelection.at(SHAPES))->GetModelMatrix());

		//Copy the shapes local to world matrix into the object constant buffer.
		shapes.at(currentSelection.at(SHAPES))->UpdateShape(shadingScene.get(), &objectConstantData, sizeof(ObjectConstantBuffer));

		//shadingScene->CopyDataIntoDynamicBuffer(OBJECTCB, shapes.at(currentSelection.at(SHAPES))->GetDrawArguments().indexOfConstantData,
			//&objectConstantData, sizeof(ObjectConstantBuffer));
	}

	void Draw()
	{
		//All the commands needed before rendering the shapes.
		shadingScene->BeforeRenderObjects(true);
		
		shadingScene->LinkPSOAndRootSignature(SHADING_PSO, 0);
	
		//Link the vertex and index buffer to the pipeline
		shadingScene->LinkStaticBuffer(FARender::VERTEX_BUFFER, SHAPES_VERTEX_BUFFER);
		shadingScene->LinkStaticBuffer(FARender::INDEX_BUFFER, SHAPES_INDEX_BUFFER);

		//Link pass constant data to the pipeline
		shadingScene->LinkDynamicBuffer(FARender::CONSTANT_BUFFER, PASSCB, 0, 1);
		
		//Link material constant data to the pipeline
		shadingScene->LinkDynamicBuffer(FARender::CONSTANT_BUFFER, MATERIALCB, 0, 2);

		//Link light constant data to the pipeline
		shadingScene->LinkDynamicBuffer(FARender::CONSTANT_BUFFER, LIGHTCB, 0, 3);

		//Get the draw arguments of the current shape.
		/*FAShapes::DrawArguments currentShapeDrawArguments{ shapes.at(currentSelection.at(SHAPES))->GetDrawArguments() };

		//Link the shapes constant data to the pipeline.
		shadingScene->LinkDynamicBuffer(FARender::CONSTANT_BUFFER, OBJECTCB, currentShapeDrawArguments.indexOfConstantData, 0);

		//Render the current shape.
		shadingScene->RenderObject(currentShapeDrawArguments.indexCount, currentShapeDrawArguments.locationOfFirstIndex,
			currentShapeDrawArguments.indexOfFirstVertex, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);*/

		shapes.at(currentSelection.at(SHAPES))->RenderShape(shadingScene.get());
		
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