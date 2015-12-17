#ifndef RENDERPASS_H
#define RENDERPASS_H

template< class RenderBackEnd, class PassType > 
class RenderPass {
public:
    bool AddPoly(){
	return false;
    }
    bool AddLight(){
	return false;
    }
    bool SetCamera(){
	return false;
    }
    bool Render(){
	return false;
    }
};

template< class PassType > 
class RenderPass< RenderBackEndOpenGL, PassType > {
public:
    bool AddPoly();
    bool AddLight();
    bool SetCamera();
    bool Render(){
	//make copy of rendering information and pass to PassType
	bool bRet = PassType::Render( glsl_program, camera, lights, poly );
	return bRet;
    }
};

template< class PassType > 
class RenderPass< RenderBackEndDirectX, PassType > {
public:
    bool AddPoly(){
	return false;
    }
    bool AddLight(){
	return false;
    }
    bool SetCamera(){
	return false;
    }
    bool Render(){
	return false;
    }
};

#endif
