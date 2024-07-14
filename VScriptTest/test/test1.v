module Vivid.Test

enum TestEnum

	DirectX,OpenGL,Vulkan;
	Other,Now

end 

class TestClass 

    func TestClass() 

        debug((TestEnum.Other+TestEnum.Now)*2);

    end 

end 


