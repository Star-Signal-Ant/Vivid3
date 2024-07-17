module Vivid.Input.Input

enum Keys

    A=65,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z

end 

class Input

    func Input()

    end 

    func GetKeyDown(int key)

        return InputKeyDown(key);

    end 

    func GetMouseMoveX()

        return InputMouseMoveX();

    end 

    func GetMouseMoveY()

        return InputMouseMoveY();

    end 

end 
