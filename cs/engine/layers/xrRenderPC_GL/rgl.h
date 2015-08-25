#pragma once

#include "../xrRender/r__dsgraph_structure.h"
#include "../xrRender/r__occlusion.h"

#include "../xrRender/PSLibrary.h"

#include "r2_types.h"
#include "gl_rendertarget.h"

#include "../xrRender/hom.h"
#include "../xrRender/detailmanager.h"
#include "../xrRender/modelpool.h"
#include "../xrRender/wallmarksengine.h"

#include "../xrRender/light_db.h"
#include "../xrRender/SkeletonCustom.h"
#include "../xrRender/LightTrack.h"

#include "../../xrEngine/irenderable.h"
#include "../../xrEngine/fmesh.h"

class  CHW
{
public:
	CHWCaps			Caps;
	GLuint			pBaseZB;
};
extern ECORE_API CHW		HW;

class CRender	:	public R_dsgraph_structure
{
public:
	enum
	{
		PHASE_NORMAL = 0,	// E[0]
		PHASE_SMAP = 1,	// E[1]
	};

	struct		_options	{
		u32		bug : 1;

		u32		ssao_blur_on : 1;
		u32		ssao_opt_data : 1;
		u32		ssao_half_data : 1;
		u32		ssao_hbao : 1;
		u32		ssao_hdao : 1;

		u32		smapsize : 16;
		u32		depth16 : 1;
		u32		mrt : 1;
		u32		mrtmixdepth : 1;
		u32		fp16_filter : 1;
		u32		fp16_blend : 1;
		u32		albedo_wo : 1;						// work-around albedo on less capable HW
		u32		HW_smap : 1;
		u32		HW_smap_PCF : 1;
		u32		HW_smap_FETCH4 : 1;

		u32		HW_smap_FORMAT : 32;

		u32		nvstencil : 1;
		u32		nvdbt : 1;

		u32		nullrt : 1;

		u32		distortion : 1;
		u32		distortion_enabled : 1;
		u32		mblur : 1;

		u32		sunfilter : 1;
		u32		sunstatic : 1;
		u32		sjitter : 1;
		u32		noshadows : 1;
		u32		Tshadows : 1;						// transluent shadows
		u32		disasm : 1;
		u32		advancedpp : 1;	//	advanced post process (DOF, SSAO, volumetrics, etc.)

		u32		forcegloss : 1;
		u32		forceskinw : 1;
		float	forcegloss_v;
	}			o;
	struct		_stats		{
		u32		l_total, l_visible;
		u32		l_shadowed, l_unshadowed;
		s32		s_used, s_merged, s_finalclip;
		u32		o_queries, o_culled;
		u32		ic_total, ic_culled;
	}			stats;
public:
	// Sector detection and visibility
	CSector*						pLastSector;
	Fvector							vLastCameraPos;
	u32								uLastLTRACK;
	xr_vector<IRender_Portal*>		Portals;
	xr_vector<IRender_Sector*>		Sectors;
	xrXRC							Sectors_xrc;
	CDB::MODEL*						rmPortals;
	CHOM							HOM;
	R_occlusion						HWOCC;

	// Global vertex-buffer container
	typedef svector<D3DVERTEXELEMENT9, MAXD3DDECLLENGTH + 1> VertexDeclarator;
	xr_vector<FSlideWindowItem>		SWIs;
	xr_vector<ref_shader>			Shaders;
	xr_vector<VertexDeclarator>		nDC, xDC;
	xr_vector<GLuint>				nVB, xVB;
	xr_vector<GLuint>				nIB, xIB;
	xr_vector<dxRender_Visual*>		Visuals;
	CPSLibrary						PSLibrary;

	CDetailManager*					Details;
	CModelPool*						Models;
	CWallmarksEngine*				Wallmarks;

	CRenderTarget*					Target;			// Render-target

	CLight_DB						Lights;
	xr_vector<light*>				Lights_LastFrame;
	light_Package					LP_normal;
	light_Package					LP_pending;

	xr_vector<Fbox3, render_alloc<Fbox3> > main_coarse_structure;

	shared_str						c_sbase;
	shared_str						c_lmaterial;
	float							o_hemi;
	float							o_hemi_cube[CROS_impl::NUM_FACES];
	float							o_sun;
	GLsync							q_sync_point[CHWCaps::MAX_GPUS];
	u32								q_sync_count;

private:
	// Loading / Unloading
	void							LoadBuffers(CStreamReader	*fs, BOOL	_alternative);
	void							LoadVisuals(IReader	*fs);
	void							LoadLights(IReader	*fs);
	void							LoadPortals(IReader	*fs);
	void							LoadSectors(IReader	*fs);
	void							LoadSWIs(CStreamReader	*fs);
	char*							LoadIncludes(LPCSTR pSrcData, UINT SrcDataLen, xr_vector<char*>& includes);

public:
	ShaderElement*					rimp_select_sh_static(dxRender_Visual	*pVisual, float cdist_sq);
	ShaderElement*					rimp_select_sh_dynamic(dxRender_Visual	*pVisual, float cdist_sq);
	IRender_Portal*					getPortal(int id) { VERIFY(!"CRender::getPortal not implemented."); return nullptr; };
	IRender_Sector*					getSectorActive();
	IRender_Sector*					detectSector(const Fvector& P, Fvector& D);
	int								translateSector(IRender_Sector* pSector) { VERIFY(!"CRender::translateSector not implemented."); return 0; };

	// HW-occlusion culling
	IC u32							occq_begin(u32&	ID)	{ return HWOCC.occq_begin(ID); }
	IC void							occq_end(u32&	ID)	{ HWOCC.occq_end(ID); }
	IC R_occlusion::occq_result		occq_get(u32&	ID)	{ return HWOCC.occq_get(ID); }

	D3DVERTEXELEMENT9*				getVB_Format(int id, BOOL _alt = FALSE)	{
		if (_alt)	{ VERIFY(id<int(xDC.size()));	return xDC[id].begin(); }
		else		{ VERIFY(id<int(nDC.size()));	return nDC[id].begin(); }
	}
	GLuint							getVB(int id, BOOL	_alt = FALSE)	{
		if (_alt)	{ VERIFY(id<int(xVB.size()));	return xVB[id]; }
		else		{ VERIFY(id<int(nVB.size()));	return nVB[id]; }
	}
	GLuint							getIB(int id, BOOL	_alt = FALSE)	{
		if (_alt)	{ VERIFY(id<int(xIB.size()));	return xIB[id]; }
		else		{ VERIFY(id<int(nIB.size()));	return nIB[id]; }
	}
	FSlideWindowItem*				getSWI(int id)			{ VERIFY(id<int(SWIs.size()));		return &SWIs[id]; }
	ICF void						apply_object(IRenderable*	O)
	{
		if (0 == O)					return;
		if (0 == O->renderable_ROS())	return;
		CROS_impl& LT = *((CROS_impl*)O->renderable_ROS());
		LT.update_smooth(O);
		o_hemi = 0.75f*LT.get_hemi();
		//o_hemi						= 0.5f*LT.get_hemi			()	;
		o_sun = 0.75f*LT.get_sun();
		CopyMemory(o_hemi_cube, LT.get_hemi_cube(), CROS_impl::NUM_FACES*sizeof(float));
	}
	IC void							apply_lmaterial()
	{
		R_constant*		C = &*RCache.get_c(c_sbase);		// get sampler
		if (0 == C)			return;
		VERIFY(RC_dest_sampler == C->destination);
		VERIFY(RC_sampler == C->type);
		CTexture*		T = RCache.get_ActiveTexture(u32(C->samp.index));
		VERIFY(T);
		float	mtl = T->m_material;
#ifdef	DEBUG
		if (ps_r2_ls_flags.test(R2FLAG_GLOBALMATERIAL))	mtl = ps_r2_gmaterial;
#endif
		RCache.hemi.set_material(o_hemi, o_sun, 0, (mtl + .5f) / 4.f);
		RCache.hemi.set_pos_faces(o_hemi_cube[CROS_impl::CUBE_FACE_POS_X],
			o_hemi_cube[CROS_impl::CUBE_FACE_POS_Y],
			o_hemi_cube[CROS_impl::CUBE_FACE_POS_Z]);
		RCache.hemi.set_neg_faces(o_hemi_cube[CROS_impl::CUBE_FACE_NEG_X],
			o_hemi_cube[CROS_impl::CUBE_FACE_NEG_Y],
			o_hemi_cube[CROS_impl::CUBE_FACE_NEG_Z]);
	}

private:
	BOOL							add_Dynamic(dxRender_Visual*pVisual, u32 planes);			// normal processing
	void							add_Static(dxRender_Visual*pVisual, u32 planes);
	void							add_leafs_Dynamic(dxRender_Visual*pVisual);					// if detected node's full visibility
	void							add_leafs_Static(dxRender_Visual*pVisual);					// if detected node's full visibility

public:
	IRender_Sector*					rimp_detectSector(Fvector& P, Fvector& D);
	void							render_main(Fmatrix& mCombined, bool _fportals);
	void							render_forward();
	void							render_menu();

public:
	// feature level
	virtual	GenerationLevel			get_generation() { return IRender_interface::GENERATION_R2; };

	virtual bool					is_sun_static() { return o.sunstatic; };
	virtual DWORD					get_dx_level() { return 0x00000000; };

	// Loading / Unloading
	virtual	void					create();
	virtual	void					destroy() { VERIFY(!"CRender::destroy not implemented."); };
	virtual	void					reset_begin();
	virtual	void					reset_end();

	virtual	void					level_Load(IReader*);
	virtual void					level_Unload();

	virtual GLuint					texture_load(LPCSTR	fname, u32& msize, GLenum& desc);
	virtual HRESULT					shader_compile(
		LPCSTR							name,
		LPCSTR                          pSrcData,
		UINT                            SrcDataLen,
		void*							pDefines,
		void*							pInclude,
		LPCSTR                          pFunctionName,
		LPCSTR                          pTarget,
		DWORD                           Flags,
		void*							ppShader,
		void*							ppErrorMsgs,
		void*							ppConstantTable);

	// Information
	virtual	void					Statistics(CGameFont* F)							{ VERIFY(!"CRender::Statistics not implemented."); };

	virtual LPCSTR					getShaderPath()									{ return "gl\\"; }
	virtual ref_shader				getShader(int id) { VERIFY(id<int(Shaders.size()));	return Shaders[id]; };
	virtual IRender_Sector*			getSector(int id) { VERIFY(id<int(Sectors.size()));	return Sectors[id]; };
	virtual IRenderVisual*			getVisual(int id) { VERIFY(id<int(Visuals.size()));	return Visuals[id]; };
	virtual IRender_Sector*			detectSector(const Fvector& P) { VERIFY(!"CRender::detectSector not implemented."); return nullptr; };
	virtual IRender_Target*			getTarget() { return Target; };

	// Main 
	IC		void					set_Frustum(CFrustum*	O)							{ VERIFY(!"CRender::set_Frustum not implemented."); VERIFY(O);	View = O; }
	virtual void					set_Transform(Fmatrix*	M) { VERIFY(!"CRender::set_Transform not implemented."); };
	virtual void					set_HUD(BOOL 		V) { VERIFY(!"CRender::set_HUD not implemented."); };
	virtual BOOL					get_HUD() { VERIFY(!"CRender::get_HUD not implemented."); return false; };
	virtual void					set_Invisible(BOOL 		V) { VERIFY(!"CRender::set_Invisible not implemented."); };
	virtual void					flush() { VERIFY(!"CRender::flush not implemented."); };
	virtual void					set_Object(IRenderable*		O) { VERIFY(!"CRender::set_Object not implemented."); };
	virtual	void					add_Occluder(Fbox2&	bb_screenspace) { VERIFY(!"CRender::add_Occluder not implemented."); };	// mask screen region as oclluded (-1..1, -1..1)
	virtual void					add_Visual(IRenderVisual*	V) { VERIFY(!"CRender::add_Visual not implemented."); };	// add visual leaf	(no culling performed at all)
	virtual void					add_Geometry(IRenderVisual*	V) { VERIFY(!"CRender::add_Geometry not implemented."); };	// add visual(s)	(all culling performed)
	virtual void					add_StaticWallmark(const wm_shader& S, const Fvector& P, float s, CDB::TRI* T, Fvector* V) { VERIFY(!"CRender::add_StaticWallmark not implemented."); };
	virtual void					add_StaticWallmark(IWallMarkArray *pArray, const Fvector& P, float s, CDB::TRI* T, Fvector* V) { VERIFY(!"CRender::add_StaticWallmark not implemented."); };
	virtual void					clear_static_wallmarks() { VERIFY(!"CRender::clear_static_wallmarks not implemented."); };
	virtual void					add_SkeletonWallmark(intrusive_ptr<CSkeletonWallmark> wm) { VERIFY(!"CRender::add_SkeletonWallmark not implemented."); };
	virtual void					add_SkeletonWallmark(const Fmatrix* xf, CKinematics* obj, ref_shader& sh, const Fvector& start, const Fvector& dir, float size) { VERIFY(!"CRender::add_SkeletonWallmark not implemented."); };
	virtual void					add_SkeletonWallmark(const Fmatrix* xf, IKinematics* obj, IWallMarkArray *pArray, const Fvector& start, const Fvector& dir, float size) { VERIFY(!"CRender::add_SkeletonWallmark not implemented."); };

	//
	virtual IBlender*				blender_create(CLASS_ID cls);
	virtual void					blender_destroy(IBlender* &);

	virtual IRender_ObjectSpecific*	ros_create(IRenderable* parent) { VERIFY(!"CRender::ros_create not implemented."); return nullptr; };
	virtual void					ros_destroy(IRender_ObjectSpecific* &) { VERIFY(!"CRender::ros_destroy not implemented."); };

	// Lighting/glowing
	virtual IRender_Light*			light_create()						{ VERIFY(!"CRender::light_create not implemented."); return nullptr; };
	virtual void					light_destroy(IRender_Light* p_)	{ VERIFY(!"CRender::light_destroy not implemented."); };
	virtual IRender_Glow*			glow_create()						{ VERIFY(!"CRender::glow_create not implemented."); return nullptr; };
	virtual void					glow_destroy(IRender_Glow* p_)		{ VERIFY(!"CRender::glow_destroy not implemented."); };

	// Models
	virtual IRenderVisual*			model_CreateParticles(LPCSTR name);
	virtual IRender_DetailModel*	model_CreateDM(IReader* F);
	IRenderVisual*					model_Create(LPCSTR name, IReader* data)		{ return Models->Create(name,data);		}
	IRenderVisual*					model_CreateChild(LPCSTR name, IReader* data)	{ return Models->CreateChild(name,data);}
	IRenderVisual*					model_Duplicate(IRenderVisual* V)				{ return Models->Instance_Duplicate((dxRender_Visual*)V);	}
	virtual void					model_Delete(IRenderVisual* &	V, BOOL bDiscard = FALSE);
	virtual void 					model_Delete(IRender_DetailModel* & F);
	virtual void					model_Logging(BOOL bEnable)						{ Models->Logging(bEnable); }
	virtual void					models_Prefetch()								{ Models->Prefetch(); }
	virtual void					models_Clear(BOOL b_complete)					{ Models->ClearPool(b_complete); }
	IRenderVisual*					model_CreatePE(LPCSTR name);

	// Occlusion culling
	virtual BOOL					occ_visible(vis_data&	V) { VERIFY(!"CRender::occ_visible not implemented."); return false; };
	virtual BOOL					occ_visible(Fbox&		B) { VERIFY(!"CRender::occ_visible not implemented."); return false; };
	virtual BOOL					occ_visible(sPoly&		P) { VERIFY(!"CRender::occ_visible not implemented."); return false; };

	// Main
	virtual void					Calculate();
	virtual void					Render();

	virtual void					Screenshot(ScreenshotMode mode = SM_NORMAL, LPCSTR name = 0) { VERIFY(!"CRender::Screenshot not implemented."); };
	virtual	void					Screenshot(ScreenshotMode mode, CMemoryWriter& memory_writer) { VERIFY(!"CRender::Screenshot not implemented."); };
	virtual void					ScreenshotAsyncBegin() { VERIFY(!"CRender::ScreenshotAsyncBegin not implemented."); };
	virtual void					ScreenshotAsyncEnd(CMemoryWriter& memory_writer) { VERIFY(!"CRender::ScreenshotAsyncEnd not implemented."); };
	virtual void					OnFrame() { VERIFY(!"CRender::OnFrame not implemented."); };

	// Render mode
	virtual void					rmNear() { VERIFY(!"CRender::rmNear not implemented."); };
	virtual void					rmFar() { VERIFY(!"CRender::rmFar not implemented."); };
	virtual void					rmNormal() { VERIFY(!"CRender::rmNormal not implemented."); };
	virtual u32						memory_usage() { VERIFY(!"CRender::memory_usage not implemented."); return 0; };

	// Constructor/destructor
	CRender();
	virtual ~CRender();
protected:
	virtual	void					ScreenshotImpl(ScreenshotMode mode, LPCSTR name, CMemoryWriter* memory_writer) { VERIFY(!"CRender::ScreenshotImpl not implemented."); };
};

extern CRender						RImplementation;
