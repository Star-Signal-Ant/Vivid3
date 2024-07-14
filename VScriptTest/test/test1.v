module Vivid.Test

enum TestEnum

	DirectX,OpenGL,Vulkan;
	Other,Now

end 

class TestClass 

    int api = TestEnum.OpenGL;

    func TestClass() 

        switch(api)
			
	case 0;

		debug("DirectX!");
	end

	case 1;

		debug("OpenGL!");
	end 

	end 

    end 

end 


