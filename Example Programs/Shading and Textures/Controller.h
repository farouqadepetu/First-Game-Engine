#pragma once

#include "Model.h"
#include "View.h"

namespace MVC
{
	class Controller
	{
	public:
		Controller();
		Controller(Model* model, View* view);
		
		Model* GetModel();
		View* GetView();

		void SetModel(Model* model);
		void SetView(View* view);

		void FrameStats();

		void UserInput();

		void Update();

		void Draw();

		int Run();

	private:
		Model* mModel;
		View* mView;
	};
}