#pragma once
#include<Windows.h>
#include<DirectXMath.h>
#include<string>
#include<vector>
#include"Camera.h"
#include"HModelRawData.h"

using namespace DirectX;

enum class ShaderType
{
	DEFAULT,
	COLORCHIP,
	WIREFRAME,
};

struct HandleInterface
{
	virtual void Delete() = 0;
};

struct HProceduralGeometry_line : public HandleInterface
{
	Vector3 dots[2];
	Color color;
	virtual void Delete() = 0;
	virtual ~HProceduralGeometry_line(){}
};

struct HProceduralGeometry_rect : public HandleInterface
{
	Vector3 dots[4];
	Color color;
	virtual void Delete() = 0;
	virtual ~HProceduralGeometry_rect() {}
};

struct HSimplePrimitive : public HandleInterface
{
	Matrix worldTM;
	virtual void Delete() = 0;
	virtual ~HSimplePrimitive() {}
};

struct HMaterialData : public HandleInterface
{
	virtual void Delete() = 0;
	virtual ~HMaterialData() {}
};

struct HSpriteData : public HandleInterface
{
	virtual void Delete() = 0;
	virtual ~HSpriteData() {}
};

struct HUIData : public HandleInterface
{
	/*If bIsAbsolutePosition is true, leftTopPosition, width, height values are absolute value.
	If bIsAbsolutePosition is false, leftTopPosition, width, height values are relative to currunt screen size.
	leftTopPosition : x and y(0 ~ 1)
	width = 0 ~ 1
	height = 0 ~ 1
	*/
	Vector3 leftTopPosition;
	Vector2 size;
	//float width;
	//float height;
	bool bIsAbsolutePosition = true;
	
	float opacity = 1.f; // 0 ~ 1

	virtual void Delete() = 0;
	virtual void SetSprite(HSpriteData* pSprite) = 0;
	virtual void SetDrawOnOff(bool onOff) = 0;
};

struct HInstanceData : public HandleInterface
{
	float materialLerpValue;
	Matrix worldTM;

	//std::unordered_map<std::string, float> animationNameAndTime;

	float animationTime;
	std::string animationName;
	float animationTimeSecond;
	std::string animationNameSecond;
	float animationLerpValue = 0;	//if this value is 0, first animation value is 100%

	virtual void Delete() = 0;
	virtual void SetMaterial(HMaterialData* pMaterial, unsigned int slot = 0) = 0;
		/**
	 * Change Shader flag.
	 *
	 * \param flag ShaderType
	 */
	virtual void SetShaderFlag(ShaderType type) = 0;
	virtual Matrix GetBoneTMAtCurruntFrame(std::string boneName) = 0;
	virtual ~HInstanceData() {}
};

struct HAnimData : public HandleInterface
{
	HAnimRawData const* pRawData;
	virtual void Delete() = 0;
	virtual ~HAnimData() {}
};

struct HModelData : public HandleInterface
{
	HModelRawData const* pRawData;
	virtual void Delete() = 0;
		/**
	* Create instance.
	*
	* \param flag ShaderType
	* 
	* \return HInstanceData ptr
	*/
	virtual HInstanceData* AddInstance(ShaderType type) = 0;
	virtual void SetAnimation(HAnimData* pAnimData) = 0;
	virtual HAnimData* GetAnimation() = 0;
	virtual ~HModelData() {}
};

class HWaveData : public HandleInterface
{
public:
	Matrix worldTM;
	virtual void Delete() = 0;
};

struct DebugString
{
	std::string message;
	Color color = Color(0,0,0);
	Vector3 pos;
	Vector2 size = Vector2(1,1);
	bool bIsAbsolutePosition = true;
};

enum LightType
{
	eDirectLight,
	ePointLight,
	eSpotLight
};

struct HLightData : public HandleInterface
{
	Vector3 strength = { 0.5f, 0.5f, 0.5f };
	float falloffStart = 1.0f;                          // point/spot light only
	Vector3 direction = { 0.0f, -1.0f, 0.0f };			// directional/spot light only
	float falloffEnd = 10.0f;                           // point/spot light only
	Vector3 position = { 0.0f, 0.0f, 0.0f };			// point/spot light only
	float spotPower = 64.0f;                            // spot light only

	virtual void Delete() = 0;
};



enum ShaderType_Primitive
{
	eNoOption_Primitive  = 0B00000000,
	eDepthOff_Primitive  = 0B00000001,
	eWireFrame_Primitive = 0B00000010,
	eAllOption_Primitive = eDepthOff_Primitive | eWireFrame_Primitive
};

class HEngine;
class HEngine_DX12_3D
{
private:
	static HEngine* m_pHEngine;
	HEngine_DX12_3D() {};
	~HEngine_DX12_3D() {};
public:

	/**
	 * singleton.
	 * 
	 * \return pHEngine
	 */
	static HEngine_DX12_3D* GetInstance();

	//----------------------------------------------------------------------------
	/* Init & Loop & Terminate Functions.
	*/  
	//----------------------------------------------------------------------------

	/**
	 * Initialize 3D Engine.
	 *
	 * \param hWnd
	 * \param clientWidth
	 * \param clientHeight
	 */
	void InitEngine(HWND hWnd, int clientWidth, int clientHeight);

	/**
	 * Loop.
	 * 
	 * This function includes all update of CPU and GPU resources and draw calls.
	 */
	void Loop();
	
	/**
	 * Terminate 3D Engine.
	 * 
	 * This function release all resources has been made.
	 * you need to call this function before termination of your app for proper
	 * memory release.
	 */
	void EndEngine();


	/**
	 * Process Window Messages.
	 * 
	 * This needs to be called to handle window Messages
	 * 
	 * 
	 * \param hWnd 
	 * \param message 
	 * \param wParam
	 * \param lParam
	 */
	void ProcessWndMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	//----------------------------------------------------------------------------
	/* Start & Finishing Engine setting.
	* When you try to make objects which are usually made during initializtion like
	* model or material, you need to call this function before & after.
	* In engine, there is a object called 'resource upload batch', which saves commands
	* to make gpu resources. It saves all the commands between StartSetting() and FinishSetting() 
	* and submits the commands to gpu at FinishSetting().
	*/
	//----------------------------------------------------------------------------

	/**
	 * Get ready for resource creation.
	 *
	 * Reset 'resource upload batch for new record of commands.
	 */
	void StartSetting();
	
	/**
	 * Excute resource creation.
	 *
	 * Submit commands to gpu for resource creation.
	 */
	void FinishSetting();

	//**list of functions that should be called between  StartSetting() and FinishSetting()
	/*
	* CreateModelFromFbxFile
	* CreateAnimationFromFbxFiles
	* CreateModelFromHModelFile
	* CreateAnimationFromHAnimFiles
	* CreateMaterial
	* CreateSprite
	* LoadSkyBox
	* LoadFont
	*/

	//----------------------------------------------------------------------------
	/* Resource Loading Functions.
	*/  
	//----------------------------------------------------------------------------

	/**
	 * Load model data & animation datas from fbx files. 
	 * 
	 * This function includes both Loading model data and animations.
	 * The string in filePath[0] is file path for model data itself. The Engine only read mesh and bone datas and
	 * skip reading animations.
	 * 
	 * This strings in filePath[1,2,3 ...] are file paths for animation datas. The Engine only reads animation datas
	 * and skip reading mesh and bone datas.
	 * 
	 * You should at least submit one string of file path otherwise it will throw error.
	 * 
	 * You should keep the returned handle to release model resources.
	 *
	 * \param filePaths filePath of a model.fbx and animations.
	 * 
	 * \return HModelData a model pointer which is used to make instances of it.
	 */
	HModelData* CreateModelFromFbxFile(std::string fbxName);
	HAnimData* CreateAnimationFromFbxFiles(std::vector<std::string> fbxNames);
	HModelData* CreateModelFromHModelFile(std::string fbxfile);
	HAnimData* CreateAnimationFromHAnimFiles(std::vector<std::string> fbxNames);

	/**
	 * Load textrue resource and make a material set.
	 *
	 * This function creates a material set from texture files.
	 * If you want to draw Instances with tesselation, you need to prepare 'height map'. otherwise you can skip
	 * 'height map' and just leave it nullptr.
	 * 
	 * \param albedo texture dds file path
	 * \param roughness texture dds file path
	 * \param metallic texture dds file path
	 * \param ao texture dds file path
	 * \param normal texture dds file path
	 * \param height texture dds file path
	 * \return material handle
	 */
	HMaterialData* CreateMaterial(const WCHAR* albedo, const WCHAR* roughness, const WCHAR* metallic,
		const WCHAR* ao, const WCHAR* normal, const WCHAR* height, const WCHAR* emissive);

	HSpriteData* CreateSprite(const WCHAR* spriteFile);
	
	//----------------------------------------------------------------------------
	/* Util functions.
	*/  
	//----------------------------------------------------------------------------

	//get delta time
	float GetElapsedTime();

	//get set camera
	void SetCamera(Camera camera);
	Camera* GetCamera();

	//get DirectXTK input
	//DirectX::Keyboard* GetKeyboard();
	//DirectX::GamePad* GetGamePad();
	//DirectX::Mouse* GetMouse();

	//output debug string, must be called everyframe. internaly saved in deque.
	void AddDebugString(DebugString debugString);

	//get pointer of  a model instance. it returns null if none is picked.
	HInstanceData* Picking(unsigned int pickX, unsigned int pickY);

	//flag = ShaderType_Primitive
	HProceduralGeometry_line* CreateLine(unsigned char flag);
	HProceduralGeometry_rect* CreateRect(unsigned char flag);
	HSimplePrimitive* CreateBox(Vector3 Size, unsigned char flag);

	//create 2D sprite UI
	HUIData* CreateUI();

	//create Light
	HLightData* CreateLight(LightType lightType);

	//default : 128, 128, 1.0f, 0.03f, 4.0f, 0.2f 
	HWaveData* CreateWave(int m, int n, float dx, float speed, float damping);

	//get client size of the window
	RECT GetClientRectFromEngine();

	//load default skybox for raytracing reflection
	void LoadSkyBox(const WCHAR* skyBoxDDSFile);

	//load default color chip for colorchip drawing
	void LoadColorChip(const WCHAR* baseColor, const WCHAR* roughness, const WCHAR* metallic, const WCHAR* emissive);


	//load default font for string output
	void LoadFont(const WCHAR* spriteFontFile);

	void SetReflectionEffect(bool bOnOff);
	void SetShadowEffect(bool bOnOff);
	void SetSSAO(bool bOnOff);
	void SetWireFrame(bool bOnOff);
	void SetDOF(bool bOnOff);
	void SetDOFParams(float focusDepth, float maxBlurDepthGap);
};