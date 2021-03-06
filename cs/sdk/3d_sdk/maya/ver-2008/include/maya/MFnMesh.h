#ifndef _MFnMesh
#define _MFnMesh
//
//-
// ==========================================================================
// Copyright (C) 1995 - 2006 Autodesk, Inc., and/or its licensors.  All 
// rights reserved.
// 
// The coded instructions, statements, computer programs, and/or related 
// material (collectively the "Data") in these files contain unpublished 
// information proprietary to Autodesk, Inc. ("Autodesk") and/or its 
// licensors,  which is protected by U.S. and Canadian federal copyright law 
// and by international treaties.
// 
// The Data may not be disclosed or distributed to third parties or be 
// copied or duplicated, in whole or in part, without the prior written 
// consent of Autodesk.
// 
// The copyright notices in the Software and this entire statement, 
// including the above license grant, this restriction and the following 
// disclaimer, must be included in all copies of the Software, in whole 
// or in part, and all derivative works of the Software, unless such copies 
// or derivative works are solely in the form of machine-executable object 
// code generated by a source language processor.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND. 
// AUTODESK DOES NOT MAKE AND HEREBY DISCLAIMS ANY EXPRESS OR IMPLIED 
// WARRANTIES INCLUDING, BUT NOT LIMITED TO, THE WARRANTIES OF 
// NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, 
// OR ARISING FROM A COURSE OF DEALING, USAGE, OR TRADE PRACTICE. IN NO 
// EVENT WILL AUTODESK AND/OR ITS LICENSORS BE LIABLE FOR ANY LOST 
// REVENUES, DATA, OR PROFITS, OR SPECIAL, DIRECT, INDIRECT, OR 
// CONSEQUENTIAL DAMAGES, EVEN IF AUTODESK AND/OR ITS LICENSORS HAS 
// BEEN ADVISED OF THE POSSIBILITY OR PROBABILITY OF SUCH DAMAGES. 
// ==========================================================================
//+
//
// CLASS:    MFnMesh
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnMesh)
//
//	Function set for polygonal objects.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnDagNode.h>
#include <maya/MVector.h>
#include <maya/MPoint.h>
#include <maya/MObject.h>
#include <maya/MObjectArray.h>
#include <maya/MColor.h>
#include <maya/MVectorArray.h>
#include <maya/MStringArray.h>
#include <maya/MColorArray.h>
#include <maya/MDGModifier.h>
#include <maya/MString.h>

// *****************************************************************************

// DECLARATIONS

class MPointArray;
class MFloatArray;
class MFloatPoint;
class MFloatPointArray;
class MFloatVector;
class MFloatVectorArray;
class MIntArray;
class MDoubleArray;
class MSelectionList;
class MMeshIsectAccelParams;
class MUintArray;

// Default tolerance
#define kMFnMeshTolerance 1.0e-3

// Point equivalent tolerance
#define kMFnMeshPointTolerance 1.0e-10

// For per-instance mesh data
#define kMFnMeshInstanceUnspecified -1


// *****************************************************************************

// CLASS DECLARATION (MFnMesh)

/// Polygonal surface function set. (OpenMaya) (OpenMaya.py)
/**

*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFnMesh : public MFnDagNode 
{

	declareDagMFn(MFnMesh, MFnDagNode);

public: 

        ///
        enum MColorRepresentation {
                ///
                kAlpha = 1,
                ///
                kRGB = 3,
                ///
                kRGBA = 4
        };

    ///
    MObject     create( int numVertices, int numPolygons,
						const MFloatPointArray &vertexArray,
                        const MIntArray &polygonCounts,
                        const MIntArray &polygonConnects,
						MObject parentOrOwner = MObject::kNullObj,
						MStatus * ReturnStatus = NULL );

    ///
    MObject     create( int numVertices, int numPolygons,
						const MPointArray 	&vertexArray,
                        const MIntArray 	&polygonCounts,
                        const MIntArray 	&polygonConnects,
						MObject parentOrOwner  = MObject::kNullObj,
						MStatus * ReturnStatus = NULL );

    ///	
	MObject		create( int numVertices, int numPolygons,
						const MFloatPointArray &vertexArray,
						const MIntArray &polygonCounts,
						const MIntArray &polygonConnects,
						const MFloatArray & uArray,
						const MFloatArray & vArray,
						MObject parentOrOwner  = MObject::kNullObj,
						MStatus * ReturnStatus = NULL );
    ///
	MObject		create( int numVertices, int numPolygons,
						const MPointArray &vertexArray,
						const MIntArray &polygonCounts,
						const MIntArray &polygonConnects,
						const MFloatArray & uArray,
						const MFloatArray & vArray,
						MObject parentOrOwner  = MObject::kNullObj,
						MStatus * ReturnStatus = NULL );
	///
	MObject		generateSmoothMesh ( MObject parentOrOwner = MObject::kNullObj,
						MStatus * ReturnStatus = NULL );
    ///
    MObject     addPolygon( const MPointArray & vertexArray,
                        	bool 		mergeVertices = true,
                        	double 		pointTolerance = kMFnMeshPointTolerance,
							MObject 	parentOrOwner = MObject::kNullObj,
							MStatus * ReturnStatus = NULL );
    ///
    MObject     addPolygon( const MPointArray & vertexArray,
							int & 		faceIndex,
                        	bool 		mergeVertices = true,
                        	double 		pointTolerance = kMFnMeshPointTolerance,
							MObject 	parentOrOwner = MObject::kNullObj,
							MStatus * ReturnStatus = NULL );

	///
	MStatus		deleteFace(int index, MDGModifier *modifier = NULL);
	///
	MStatus		deleteEdge(int index, MDGModifier *modifier = NULL);
	///
	MStatus		deleteVertex(int index, MDGModifier *modifier = NULL);


	// Poly Operations
	///
	enum SplitPlacement {
        ///
		kOnEdge,
        ///
		kInternalPoint,
        ///
		kInvalid
    };
	///
	MStatus     split(MIntArray & placements,
					  MIntArray & edgeList,
					  MFloatArray & edgeFactors,
					  MFloatPointArray & internalPoints);
	///
	MStatus		subdivideFaces(MIntArray & faceList,
							   int divisionCount);
	///
	MStatus     subdivideEdges(MIntArray & edgeList,
							   int divisionCount);
	///
	MStatus		extrudeFaces(MIntArray & faceList,
							 int extrusionCount,
							 MFloatVector* translation,
							 bool extrudeTogether);
	///
	MStatus		extrudeEdges(MIntArray & edgeList,
							 int extrusionCount,
							 MFloatVector* translation,
							 bool extrudeTogether);
	///
	MStatus		duplicateFaces(MIntArray & faceList,
							   MFloatVector* translation);
	///
	MStatus		extractFaces(MIntArray & faceList,
							 MFloatVector* translation);
	///
	MStatus		collapseFaces(MIntArray & faceList);
	///
	MStatus		collapseEdges(MIntArray & edgeList);

	// Counts
    ///
	int        numVertices( MStatus * ReturnStatus = NULL) const;
    ///
	int        numEdges( MStatus * ReturnStatus = NULL ) const;
    ///
	int        numPolygons( MStatus * ReturnStatus = NULL ) const;
    ///
	int		   numFaceVertices( MStatus * ReturnStatus = NULL ) const;
	///
	int        polygonVertexCount( int polygonId,
									MStatus * ReturnStatus = NULL ) const;
	///
	int		   numUVs( MStatus * ReturnStatus = NULL ) const;
	///
	int		   numUVs( const MString &uvSet, 
					   MStatus * ReturnStatus = NULL ) const;
	///
	int		   numColors( MStatus * ReturnStatus = NULL ) const;
	///
	int		   numColors( const MString &colorSet, 
					   MStatus * ReturnStatus = NULL ) const;
	///
	int		   numNormals( MStatus * ReturnStatus = NULL ) const;

	///
	bool		   hasColorChannels( const MString &colorSet, MStatus * ReturnStatus = NULL ) const;

	///
	bool		   hasAlphaChannels( const MString &colorSet, MStatus * ReturnStatus = NULL ) const;

	///
	MColorRepresentation  getColorRepresentation( const MString &colorSet, MStatus * ReturnStatus = NULL) const;

	///
	bool		   isColorClamped( const MString &colorSet, MStatus * ReturnStatus = NULL ) const;

	///
	MStatus		   setIsColorClamped( const MString &colorSet, bool clamped);

	///
    MStatus     getTriangles( MIntArray & triangleCounts, MIntArray &triangleVertices ) const;

	//	Intersection methods with acceleration options
	///
	static MMeshIsectAccelParams uniformGridParams( int xDiv, int yDiv, int zDiv );

	///
	static MMeshIsectAccelParams autoUniformGridParams();

	///
	bool	closestIntersection(	const MFloatPoint& 	raySource,
									const MFloatVector& rayDirection,
									const MIntArray* 	faceIds,
									const MIntArray* 	triIds,
										  bool  		idsSorted,
										  MSpace::Space space,
										  float 		maxParam,
										  bool  		testBothDirections,
										  MMeshIsectAccelParams *accelerator,
										  MFloatPoint& 	hitPoint,
										  float* 		hitRayParam,
										  int* 			hitFace,
										  int* 			hitTriangle,
										  float*		hitBary1,
										  float*		hitBary2,
										  float			tolerance = 1e-6,
										  MStatus*		ReturnStatus = NULL );
		
	///								  
	bool	anyIntersection(		const MFloatPoint& 	raySource,
									const MFloatVector& rayDirection,
									const MIntArray* 	faceIds,
									const MIntArray* 	triIds,
										  bool  		idsSorted,
										  MSpace::Space	space,
										  float 		maxParam,
										  bool  		testBothDirections,
										  MMeshIsectAccelParams *accelerator,
										  MFloatPoint& 	hitPoint,
										  float* 		hitRayParam,
										  int* 			hitFace,
										  int* 			hitTriangle,
										  float*		hitBary1,
										  float*		hitBary2,
										  float			tolerance = 1e-6,
										  MStatus*		ReturnStatus = NULL );
	///
	bool	allIntersections(		const MFloatPoint& 			raySource,
									const MFloatVector& 		rayDirection,
									const MIntArray* 			faceIds,
									const MIntArray* 			triIds,
										  bool  				idsSorted,
										  MSpace::Space			space,
										  float 				maxParam,
										  bool  				testBothDirections,
										  MMeshIsectAccelParams *accelerator,
										  bool					sortHits,
										  MFloatPointArray& 	hitPoints,
										  MFloatArray* 			hitRayParams,
										  MIntArray*  			hitFaces,
										  MIntArray* 			hitTriangles,
										  MFloatArray*			hitBary1,
										  MFloatArray*			hitBary2,
										  float					tolerance = 1e-6,
										  MStatus*				ReturnStatus = NULL );

	///
	MStatus		sortIntersectionFaceTriIds(	MIntArray* faceIds, MIntArray* triIds );
	
	///
	MStatus		freeCachedIntersectionAccelerator();
	
	///
	MString		cachedIntersectionAcceleratorInfo( MStatus *ReturnStatus = NULL );
	
	///
	static MString		globalIntersectionAcceleratorsInfo();

	///
	static void			clearGlobalIntersectionAcceleratorInfo();

	// Intersection/getClosestPoint methods.
	///
	bool     	intersect( const MPoint & raySource,
                            const MVector & rayDirection,
							MPointArray & points,
                            double tolerance = kMFnMeshPointTolerance,
							MSpace::Space space = MSpace::kObject,
							MIntArray * polygonIds = NULL,
							MStatus * ReturnStatus = NULL ) const;        
	///
    MStatus     getClosestPointAndNormal( const MPoint &  toThisPoint,
                			MPoint & theClosestPoint,
                			MVector & theNormal,
							MSpace::Space space = MSpace::kObject,
							int * closestPolygon = NULL ) const;
	///
    MStatus     getClosestPoint(const MPoint &toThisPoint,
                            MPoint &theClosestPoint,
							MSpace::Space space = MSpace::kObject,
							int * closestPolygon = NULL ) const;

	///
    MStatus     getClosestNormal(const MPoint &toThisPoint,
                            MVector & theNormal,
							MSpace::Space space = MSpace::kObject,
							int * closestPolygon = NULL ) const;


	// Shaders and membership methods.
	///
    MStatus     getConnectedShaders(
                            unsigned int instanceNumber,
                            MObjectArray & shaders,
							MIntArray & indices ) const;
	///
	MStatus		getConnectedSetsAndMembers(
							unsigned int instanceNumber,
							MObjectArray & sets,
							MObjectArray & comps,
							bool renderableSetsOnly ) const;

	///
	MObject		copy( const MObject & source,
					  MObject parentOrOwner = MObject::kNullObj,
					  MStatus * ReturnStatus = NULL );
    ///
	MStatus		updateSurface();
	///
	MStatus		syncObject();


	// Get/set methods for mesh vertices.
	///
	MStatus 	getPoints( MFloatPointArray & vertexArray,
							MSpace::Space space = MSpace::kObject ) const;
	///
	MStatus 	getPoints( MPointArray & vertexArray,
							MSpace::Space space = MSpace::kObject ) const;
	///
	MStatus 	setPoints( MFloatPointArray & vertexArray,
							MSpace::Space space = MSpace::kObject );
	///
	MStatus 	setPoints( MPointArray & vertexArray,
							MSpace::Space space = MSpace::kObject );
	///
	MStatus 	getVertices( MIntArray & vertexCount,
							 MIntArray & vertexList ) const;
	///
	MStatus 	getPolygonVertices( int polygonId,
							MIntArray & vertexList ) const;
	///
	MStatus	    getPolygonTriangleVertices( int polygonId, 
											int triangleId, 
			   								int triangleVertices[3] ) const;
	///
	MStatus		setPoint( int vertexId, const MPoint & pos,
							MSpace::Space space = MSpace::kObject );
	///
	MStatus		getPoint( int vertexId, MPoint & pos,
							MSpace::Space space = MSpace::kObject ) const;


	// Methods for setting/getting Normals
	///
	MStatus 	getNormals( MFloatVectorArray& normals,
							MSpace::Space space = MSpace::kObject ) const;
	///
	MStatus 	setNormals( MFloatVectorArray& normals,
							MSpace::Space space = MSpace::kObject );
	///
    MStatus     getFaceVertexNormal( int faceIndex, int vertexIndex,
                            MVector & normal,
                            MSpace::Space space = MSpace::kObject ) const;
    ///
    MStatus     getFaceVertexNormals( int faceIndex, MFloatVectorArray & normals,
                            MSpace::Space space = MSpace::kObject ) const;
    ///
    MStatus     getNormalIds( MIntArray & normalIdCounts, MIntArray &normalIds ) const;
    ///
    MStatus     getFaceNormalIds( int faceIndex, MIntArray &normals ) const;
	///
	MStatus		setFaceVertexNormal( MVector &normal,
									int faceIndex, int vertexIndex,
									MSpace::Space space = MSpace::kObject,
									MDGModifier *modifier = NULL);
	///
	MStatus		setVertexNormal( MVector &normal,
									int vertexIndex,
									MSpace::Space space = MSpace::kObject,
									MDGModifier *modifier = NULL);
	///
	MStatus		setFaceVertexNormals( MVectorArray & normalArray,
									MIntArray & faceList, MIntArray & vertexList,
									MSpace::Space space = MSpace::kObject );
	///
	MStatus		setVertexNormals( MVectorArray & normalArray,
									MIntArray & vertexList,
									MSpace::Space space = MSpace::kObject );
	///
	MStatus		getVertexNormal( int vertexId, MVector & normal,
							MSpace::Space space = MSpace::kObject ) const;
	///
	MStatus		getPolygonNormal( int polygonId, MVector & normal,
							MSpace::Space space = MSpace::kObject ) const;
	///
	bool		isNormalLocked(int normalId, MStatus* ReturnStatus = NULL ) const;
	///
	MStatus		lockVertexNormals(MIntArray & vertexList);
	///
	MStatus		lockFaceVertexNormals(MIntArray & faceList, MIntArray & vertexList);
	///
	MStatus		unlockVertexNormals(MIntArray & vertexList);
	///
	MStatus		unlockFaceVertexNormals( MIntArray & faceList, MIntArray & vertexList);

	// Methods for getting tangents and binormals
	///
    int			getTangentId( int faceIndex, int vertexIndex, MStatus * ReturnStatus = NULL ) const;
	///
	MStatus 	getTangents( MFloatVectorArray& normals,
							MSpace::Space space = MSpace::kObject,
							const MString * uvSet = NULL) const;
	///
    MStatus     getFaceVertexTangent( int faceIndex, int vertexIndex,
                            MVector & tangent,
                            MSpace::Space space = MSpace::kObject,
							const MString * uvSet = NULL ) const;
    ///
    MStatus     getFaceVertexTangents( int faceIndex, MFloatVectorArray & tangents,
                            MSpace::Space space = MSpace::kObject,
							const MString * uvSet = NULL ) const;
	///
	MStatus 	getBinormals( MFloatVectorArray& normals,
							MSpace::Space space = MSpace::kObject,
							const MString * uvSet = NULL ) const;
	///
    MStatus     getFaceVertexBinormal( int faceIndex, int vertexIndex,
                            MVector & normal,
                            MSpace::Space space = MSpace::kObject,
							const MString * uvSet = NULL) const;
    ///
    MStatus     getFaceVertexBinormals( int faceIndex, MFloatVectorArray & normals,
                            MSpace::Space space = MSpace::kObject,
							const MString * uvSet = NULL) const;


	// Face info methods.
	///
	bool		isPolygonConvex( int faceIndex, MStatus * ReturnStatus = NULL ) const;

	// Edge info methods.
	///
	MStatus 	getEdgeVertices( int edgeId, int2 & vertexList ) const;
	///
	bool		isEdgeSmooth( int edgeId, MStatus * ReturnStatus = NULL ) const;
	///
	MStatus		setEdgeSmoothing( int edgeId, bool smooth = true );
	///
	MStatus		cleanupEdgeSmoothing();
	///
	MStatus		getCreaseEdges(MUintArray &edgeIds, MDoubleArray &creaseData) const;
	///
	MStatus		setCreaseEdges(const MUintArray &edgeIds, const MDoubleArray &creaseData);

	// Vertex info methods.
	///
	MStatus		getCreaseVertices(MUintArray &vertexIds, MDoubleArray &creaseData) const;
	///
	MStatus		setCreaseVertices(const MUintArray &vertexIds, const MDoubleArray &creaseData);


	// UVs operators and methods.
	///
	int			numUVSets( MStatus *ReturnStatus = NULL) const;
	///
	MString		createUVSetWithName(const MString &uvSetName,
									MDGModifier *modifier = NULL,
									MStatus *ReturnStatus = NULL,
									const MUintArray *instances = NULL);

	MString		createUVSetDataMeshWithName(const MString &uvSetName, MStatus *ReturnStatus = NULL);

	///
	MString		copyUVSetWithName(const MString &fromName, const MString &toName,
						  MDGModifier *modifier = NULL, MStatus *ReturnStatus = NULL);
	///
	MStatus		renameUVSet(const MString &origName,
							const MString &newName,
							MDGModifier *modifier = NULL);
	///
	MStatus		deleteUVSet(const MString &setName,
							 MDGModifier *modifier = NULL,
							 MSelectionList *currentSelection = NULL);
	///
	MStatus		setCurrentUVSetName(const MString &setName,
									 MDGModifier *modifier = NULL,
									MSelectionList *currentSelection = NULL);
	///
	MString		currentUVSetName(MStatus *ReturnStatus = NULL,
								 int instance = kMFnMeshInstanceUnspecified) const;
	///
	MStatus		getUVSetNames(MStringArray &setNames) const;
	///
	MStatus		getUVSetFamilyNames(MStringArray &familyNames) const;
	///
	MStatus		getUVSetsInFamily(const MString& familyName,
								  MStringArray &setNames) const;
	///
	bool        isUVSetPerInstance( const MString& name,
									MStatus *ReturnStatus = NULL) const;	
	///
	MStatus		getFaceUVSetNames( int polygonId, MStringArray &setNames) const;
	///
	MStatus		getAssociatedUVSetTextures(const MString uvSetName,
										   MObjectArray & textures) const;
	///
	MStatus		getAssociatedUVSetInstances(const MString& uvSetName,
											MIntArray& instances) const;
	///
	MStatus 	setUVs( const MFloatArray& uArray, const MFloatArray& vArray,
						const MString * uvSet = NULL );
	///
	MStatus 	setSomeUVs( const  MIntArray & uvIds, const MFloatArray& uArray,
						    const MFloatArray& vArray, const MString * uvSet = NULL );
	///
	MStatus 	getUVs( MFloatArray& uArray, MFloatArray& vArray,
						const MString * uvSet = NULL ) const;
	///
	MStatus		setUV( int uvId, float u, float v,
					   const MString * uvSet = NULL );
	///
	MStatus		getUV( int uvId, float & u, float & v,
					   const MString * uvSet = NULL ) const;

	///
	MStatus     getPointAtUV( int polygonId, MPoint &theClosestPoint, float2 & uvPoint,
                       MSpace::Space space = MSpace::kObject,
                       const MString * uvSet = NULL, float tolerance=0.0);
	///
	MStatus     getUVAtPoint( MPoint &pt, float2 & uvPoint,
                       MSpace::Space space = MSpace::kObject,
                       const MString * uvSet = NULL, int * closestPolygon = NULL);

	///
	MStatus 	getPolygonUV( int polygonId, int vertexIndex, 
								float & u, float & v,
							  const MString * uvSet = NULL ) const;
	///
	MStatus 	getPolygonUVid( int polygonId, int vertexIndex, 
								int &uvId,
								const MString * uvSet = NULL ) const;
	///
	MStatus 	assignUV( int polygonId, int vertexIndex, int uvId,
						  const MString * uvSet = NULL );
	///
	MStatus		assignUVs( const MIntArray& uvCounts, const MIntArray& uvIds,
						   const MString * uvSet = NULL );
	///
	MStatus		clearUVs( const MString * uvSet = NULL );
	///
	MStatus		getAssignedUVs( MIntArray& uvCounts, MIntArray& uvIds,
								const MString * uvSet = NULL ) const;

	MStatus		getUvShellsIds( MIntArray& uvShellIds, 
								unsigned int & nbUvShells,
								const MString * uvSet = NULL ) const;

	// Color Per Vertex Methods
	///
	int			numColorSets( MStatus *ReturnStatus = NULL) const;
	///
	MString		createColorSetWithName(const MString &colorSetName,
							MDGModifier *modifier = NULL,
							MStatus *ReturnStatus = NULL);

	///
	MStatus		deleteColorSet(const MString &colorSetName,
							 MDGModifier *modifier = NULL,
							 MSelectionList *currentSelection = NULL);
	///
	MStatus		setCurrentColorSetName(const MString &setName,
									 MDGModifier *modifier = NULL,
									 MSelectionList *currentSelection = NULL);
	///
	MString		currentColorSetName(MStatus * ReturnStatus = NULL) const;
	///
	MStatus		getColorSetNames(MStringArray &setNames) const;
	///
	MStatus		setFaceColor(MColor &color, int index);
	///
	MStatus		setFaceColor(MColor &color, int index, MColorRepresentation rep);
	///
	MStatus		setVertexColor(MColor &color, int index, MDGModifier *modifier = NULL);
	///
	MStatus		setVertexColor(MColor &color, int index, MDGModifier *modifier, MColorRepresentation rep);
	///
	MStatus		setFaceVertexColor( MColor &color, int faceIndex, int vertexIndex,  
									MDGModifier *modifier = NULL);
	///
	MStatus		setFaceVertexColor( MColor &color, int faceIndex, int vertexIndex,  
					    MDGModifier *modifier, MColorRepresentation rep);
	///
	MStatus		setFaceVertexColor(  int faceIndex, int localVertexIndex, MColor &color,  
									MDGModifier *modifier = NULL);
	///
	MStatus		setFaceVertexColor(  int faceIndex, int localVertexIndex, MColor &color,  
					     MDGModifier *modifier,
					     MColorRepresentation rep);
	///
	MStatus		setFaceColors(MColorArray &colors, MIntArray &faceList);
	///
	MStatus		setFaceColors(MColorArray &colors, MIntArray &faceList, MColorRepresentation rep);
	///
	MStatus		setVertexColors(MColorArray &colors, MIntArray &vertexList,
								MDGModifier *modifier = NULL);
	///
	MStatus		setVertexColors(MColorArray &colors, MIntArray &vertexList,
					MDGModifier *modifier,
					MColorRepresentation rep);
	///
	MStatus		setFaceVertexColors(MColorArray &colors, 
									MIntArray &faceList, MIntArray &vertexList,
									MDGModifier *modifier = NULL);
	///
	MStatus		setFaceVertexColors(MColorArray &colors, 
					    MIntArray &faceList, MIntArray &vertexList,
					    MDGModifier *modifier,
					    MColorRepresentation rep);
	///
	MStatus		removeFaceColors(MIntArray &faceList);
	///
	MStatus		removeVertexColors(MIntArray &vertexList);
	///
	MStatus		removeFaceVertexColors( MIntArray &faceList, MIntArray &vertexList);
	///
	MStatus		getVertexColors(MColorArray &colors, const MString * colorSet = NULL, const MColor * defaultUnsetColor = NULL);
	///
	MStatus		getFaceVertexColors(MColorArray &colors,  const MString *colorSet = NULL, const MColor * defaultUnsetColor = NULL);
	///
	MStatus		getFaceVertexColorIndex(int faceIndex, int localVertexIndex,
										int &colorIndex, const MString * colorSet = NULL);
	///
	MStatus 	setColors( const MColorArray& colors, const MString* colorSet = NULL );
	///
	MStatus 	setColors( const MColorArray& colors, const MString* colorSet, MColorRepresentation rep );
	///
	MStatus 	setSomeColors( const  MIntArray& colorIds, const MColorArray& colors,
								const MString* colorSet = NULL );
	///
	MStatus 	setSomeColors( const  MIntArray& colorIds, const MColorArray& colors,
				       const MString* colorSet, MColorRepresentation rep );
	///
	MStatus 	getColors( MColorArray& colors, const MString * colorSet = NULL , const MColor *defaultUnsetColor = NULL) const;
	///
	MStatus		getColorIndex(int faceIndex, int localVertexIndex,
							  int &colorIndex, const MString *colorSet = NULL ) ;
	///
	MStatus		setColor( int colorId, const MColor &color,
					   const MString * colorSet = NULL );
	///
	MStatus		setColor( int colorId, const MColor &color,
				  const MString * colorSet,
				  MColorRepresentation rep );
	///
	MStatus		getColor( int colorId, MColor &color,
				  const MString * colorSet = NULL,
				  const MColor * defaultUnsetColor = NULL) const;
	///
	MStatus 	assignColor( int polygonId, int vertexIndex, int colorId,
						  const MString* colorSet = NULL );
	///
	MStatus		assignColors( const MIntArray& colorIds,
						   const MString* colorSet = NULL );
	///
	MStatus		clearColors( const MString* colorSet = NULL );


	// Polygon Hole Query Method
	///
	int			getHoles( MIntArray& holeInfoArray, 
						  MIntArray& holeVertexArray, 
						  MStatus * ReturnStatus = NULL );

	///
	bool		onBoundary( int polygonId, MStatus * ReturnStatus = NULL) const;

	// Blind Data Query Methods
	///
	bool		isBlindDataTypeUsed( int blindDataId,
								   MStatus * ReturnStatus = NULL ) const;
	///
	MStatus		createBlindDataType( int blindDataId,
									 MStringArray longNames,
									 MStringArray shortNames,
									 MStringArray formatNames);
	///
	bool		hasBlindDataComponentId( int compID,
							  MFn::Type compType,
							  int blindDataId,
							  MStatus * ReturnStatus = NULL ) const;
	///
	bool		hasBlindData( MFn::Type compType,
							  int blindDataId,
							  MStatus * ReturnStatus = NULL ) const;
	///
	bool		hasBlindDataComponentId( int compID,
							  MFn::Type compType,
							  MStatus * ReturnStatus = NULL ) const;
	///
	bool		hasBlindData( MFn::Type compType,
							  MStatus * ReturnStatus = NULL ) const;
	// Get Blind Data Methods.
	///
	MStatus		getBlindDataTypes(MFn::Type compType,
							MIntArray& blindDataIds ) const;

	///
	MStatus		getBlindDataAttrNames(int blindDataId,
							 MStringArray& longNames,
							 MStringArray& shortNames,
							 MStringArray& formatNames) const;

	///
	MStatus		getFaceVertexBlindDataIndex(int faceIndex,
							  int vertexIndex,
							  int &blindDataIndex) const;
	///
    MStatus		getBlindDataFaceVertexIndices(int blindDataIndex,
							int &faceIndex,
							int &vertexIndex) const;
	///
	MStatus		getIntBlindData( int compID, MFn::Type compType,
							  int blindDataId, MString attrName,
							  int &data) const;
	///
	MStatus		getIntBlindData( MFn::Type compType,
							  int blindDataId, MString attrName,
							  MIntArray& compIDs, MIntArray& data) const;
	///
	MStatus		getFloatBlindData( int compID, MFn::Type compType,
							  int blindDataId, MString attrName,
							  float &data) const;
	///
	MStatus		getFloatBlindData( MFn::Type compType,
							  int blindDataId, MString attrName,
							  MIntArray& compIDs, MFloatArray& data) const;
	///
	MStatus		getDoubleBlindData( int compID, MFn::Type compType,
							  int blindDataId, MString attrName,
							  double &data) const;
	///
	MStatus		getDoubleBlindData( MFn::Type compType,
							  int blindDataId, MString attrName,
							  MIntArray& compIDs, MDoubleArray& data) const;
	///
	MStatus		getBoolBlindData( int compID, MFn::Type compType,
							  int blindDataId, MString attrName,
							  bool &data) const;
	///
	MStatus		getBoolBlindData( MFn::Type compType,
							  int blindDataId, MString attrName,
							  MIntArray& compIDs, MIntArray& data) const;
	///
	MString		stringBlindDataComponentId( int compID, MFn::Type compType,
							  int blindDataId, MString attrName, 
							  MStatus *ReturnStatus = NULL) const;
	///
	MStatus		getStringBlindData( MFn::Type compType,
							  int blindDataId, MString attrName,
							  MIntArray& compIDs, MStringArray& data) const;
	///
	MString		binaryBlindDataComponentId( int compID, MFn::Type compType,
							  int blindDataId, MString attrName,
							  MStatus *ReturnStatus = NULL) const;
	///
	MStatus		getBinaryBlindData( MFn::Type compType,
							  int blindDataId, MString attrName,
							  MIntArray& compIDs, MStringArray& data) const;

	// Set Blind Data Methods
	///
	MStatus		setIntBlindData( int compID, MFn::Type compType,
							  int blindDataId,
							  MString attrName,
							  int data);
	///
	MStatus		setIntBlindData( MIntArray& compIDs, MFn::Type compType,
							  int blindDataId,
							  MString attrName,
							  int data);
	///
	MStatus		setIntBlindData( MIntArray& compIDs, MFn::Type compType,
							  int blindDataId,
							  MString attrName,
							  MIntArray& data);
	///
	MStatus		setFloatBlindData( int compID, MFn::Type compType,
							  int blindDataId,
							  MString attrName,
							  float data);
	///
	MStatus		setFloatBlindData( MIntArray& compIDs, MFn::Type compType,
							  int blindDataId,
							  MString attrName,
							  float data);
	///
	MStatus		setFloatBlindData( MIntArray& compIDs, MFn::Type compType,
							  int blindDataId,
							  MString attrName,
							  MFloatArray &data);
	///
	MStatus		setDoubleBlindData( int compID, MFn::Type compType,
							  int blindDataId,
							  MString attrName,
							  double data);
	///
	MStatus		setDoubleBlindData( MIntArray& compIDs, MFn::Type compType,
							  int blindDataId,
							  MString attrName,
							  double data);
	///
	MStatus		setDoubleBlindData( MIntArray& compIDs, MFn::Type compType,
							  int blindDataId,
							  MString attrName,
							  MDoubleArray &data);
	///
	MStatus		setBoolBlindData( int compID, MFn::Type compType,
							  int blindDataId,
							  MString attrName,
							  bool data);
	///
	MStatus		setBoolBlindData( MIntArray& compIDs, MFn::Type compType,
							  int blindDataId,
							  MString attrName,
							  bool data);
	///
	MStatus		setBoolBlindData( MIntArray& compIDs, MFn::Type compType,
							  int blindDataId,
							  MString attrName,
							  MIntArray& data);
	///
	MStatus		setStringBlindData( int compID, MFn::Type compType,
							  int blindDataId,
							  MString attrName,
							  MString data);
	///
	MStatus		setStringBlindData( MIntArray& compIDs, MFn::Type compType,
							  int blindDataId,
							  MString attrName,
							  MString data);
	///
	MStatus		setStringBlindData( MIntArray& compIDs, MFn::Type compType,
							  int blindDataId,
							  MString attrName,
							  MStringArray &data);
	///
	MStatus		setBinaryBlindData( int compID, MFn::Type compType,
							  int blindDataId,
							  MString attrName,
							  MString data);
	///
	MStatus		setBinaryBlindData( MIntArray& compIDs, MFn::Type compType,
							  int blindDataId,
							  MString attrName,
							  MString data);
	///
	MStatus		setBinaryBlindData( MIntArray& compIDs, MFn::Type compType,
							  int blindDataId,
							  MString attrName,
							  MStringArray &data);
    ///
    MStatus     setBinaryBlindData( int compID, MFn::Type compType,
                              int blindDataId,
                              MString attrName,
                              char *data, int length);
	///
	MStatus     clearBlindData(int compID, MFn::Type compType,
							  int blindDataId,
							  MString attrName);
	///
	MStatus     clearBlindData(MFn::Type compType,
							  int blindDataId,
							  MString attrName);

	///
	MStatus		clearBlindData(MFn::Type compType);
	///
	MStatus     clearBlindData(int compID,
							  MFn::Type compType,
							  int blindDataId);
	///
	MStatus     clearBlindData(MFn::Type compType,
							  int blindDataId);

	///
	void	setCheckSamePointTwice( bool check = true );
	///
	bool	getCheckSamePointTwice( void );

	///
	MStatus 	createInPlace( int numVertices, 
			int numPolygons, 
			const MFloatPointArray &vertexArray, 
			const MIntArray &polygonCounts, 
			const MIntArray &polygonConnects );

	///
	MStatus		copyInPlace( const MObject& source );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnMesh, MFnDagNode );

    /// OBSOLETE & NO SCRIPT SUPPORT
    MObject     addPolygon( MPointArray & vertexArray,
                        	double 		polyTolerance,
                        	bool     	mergeVertices = true,
                        	double 		pointTolerance = kMFnMeshPointTolerance,
                        	bool     	forceGeometry = true,
							MObject 	parentOrOwner = MObject::kNullObj,
							MStatus * ReturnStatus = NULL );

	/// OBSOLETE & NO SCRIPT SUPPORT
	bool     	intersect( MPoint & raySource, MVector & rayDirection,
							MPointArray & points,
							MStatus * ReturnStatus = NULL ) const;        

	/// OBSOLETE & NO SCRIPT SUPPORT
	bool     	intersect( const MPoint & raySource,
                            const MVector & rayDirection,
							MPointArray & points,
							MStatus * ReturnStatus = NULL ) const;

	/// OBSOLETE & NO SCRIPT SUPPORT
	MStatus		getBlindDataAttrNames(int blindDataId,
							 MStringArray& longNames,
							 MStringArray& shortNames) const;

	/// OBSOLETE & NO SCRIPT SUPPORT
	bool		hasBlindData( int compID,
							  MFn::Type compType,
							  int blindDataId,
							  MStatus * ReturnStatus = NULL ) const;
	/// OBSOLETE & NO SCRIPT SUPPORT
	bool		hasBlindData( int compID,
							  MFn::Type compType,
							  MStatus * ReturnStatus = NULL ) const;
	
	///		NO SCRIPT SUPPORT
	MStatus		createUVSet(MString &uvSetName,
							MDGModifier *modifier = NULL,
							const MUintArray *instances = NULL);

	MStatus		createUVSetDataMesh(MString &uvSetName);
	///		NO SCRIPT SUPPORT
	MStatus		copyUVSet(const MString &fromName, MString &toName,
						  MDGModifier *modifier = NULL);
	///		NO SCRIPT SUPPORT
	MStatus		getCurrentUVSetName(MString &setName,
									int instance = kMFnMeshInstanceUnspecified) const;
	///		NO SCRIPT SUPPORT
	MStatus		createColorSet(MString &colorSetName,
							   MDGModifier *modifier = NULL);
	///		NO SCRIPT SUPPORT
	MStatus		createColorSet(MString &colorSetName,
				       MDGModifier *modifier, 
				       bool clamped,
				       MColorRepresentation rep);
	///		NO SCRIPT SUPPORT
	MStatus		getCurrentColorSetName(MString &setName) const;
	///		NO SCRIPT SUPPORT
	MStatus		getStringBlindData( int compID, MFn::Type compType,
							  int blindDataId, MString attrName,
							  MString &data) const;
	///		NO SCRIPT SUPPORT
	MStatus		getBinaryBlindData( int compID, MFn::Type compType,
							  int blindDataId, MString attrName,
							  MString &data) const;

END_NO_SCRIPT_SUPPORT:

protected:  
	virtual bool	objectChanged( MFn::Type tp,
								MStatus * ReturnStatus );

    bool			getUVSetIndex( const MString * uvSetName,
								   int & uvSet) const;
	bool			getColorSetIndex( const MString * colorSetName,
									  int &colorSet ) const;

	bool			getMeshForIntersection( MSpace::Space, MFloatPoint&, MFloatVector&,
										void*&, MStatus* );

public:
	MStatus		createColorSetDataMesh(MString &colorSetName);

private:
// Private members
	void *f_meshRef;
	bool f_checkSamePoint;

};

//	Class to define parameters for accelerated intersection operations.
//	Use MFnMesh::uniformGridParams() or MFnMesh::autoUniformGridParams()
//	to create one of these to pass into the allIntersections(), 
//	closestIntersection(), and anyIntersection() methods
//
class OPENMAYA_EXPORT MMeshIsectAccelParams
{
public:
		MMeshIsectAccelParams(){}
		
private:

		int type;
		int xDiv;
		int yDiv;
		int zDiv;
		bool verbose;
};


#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MFnMesh */
