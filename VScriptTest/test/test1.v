module Vivid.Test

enum TestEnum

	DirectX,OpenGL,Vulkan;
	Other,Now

end 

class TestClass 

    int api = TestEnum.OpenGL;

    func TestClass() 

    switch(api)
			
	case TestEnum.DirectX:

		debug("DirectX!");
		break;

	case TestEnum.OpenGL:

		debug("OpenGL!");
		break;

	end 

    end 

end 


