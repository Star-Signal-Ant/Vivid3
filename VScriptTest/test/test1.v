module Vivid.Test

enum TestEnum

	DirectX,OpenGL,Vulkan;
	Other,Now

end 

class TestClass 

    int api = TestEnum.OpenGL;


	func Check()

		
	if(api = TestEnum.DirectX)

		debug("API=OpenGL");

	end 

		   switch(api)
			
	case TestEnum.DirectX:

		debug("DirectX!");
		break;

	case TestEnum.OpenGL:

		debug("OpenGL!");
		return 300;

	end 

	return 35;		

	end 

    func TestClass() 

		debug(Check());

    end 

end 


