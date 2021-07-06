#include "FBXManager.h"

static FBXManager* fbxManager;


FBXManager* FBXManager::getFbxManager() {
	if (!fbxManager) {

		fbxManager = new FBXManager();
	}
	return fbxManager;

}

char* FBXManager::getFileName() {
	return fileName;
}

bool FBXManager::loadFbx(char* path)
{
	// Create an importer using the SDK manager.
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

	if (!lImporter->Initialize(path, -1, lSdkManager->GetIOSettings())) {
		printf("Call to FbxImporter::Initialize() failed.\n");
		printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
		loaded = false;

	}
	else {

		if (lScene) {
			lScene->Destroy();
		}

		// Create a new scene so that it can be populated by the imported file.
		lScene = FbxScene::Create(lSdkManager, "myScene");

		// Import the contents of the file into the scene.
		loaded = lImporter->Import(lScene);
		// The file is imported, so get rid of the importer.
		lImporter->Destroy();
	}

	return loaded;
}

void FBXManager::drawSceneGL()
{
	setAmbientColor(.1f, .1f, .1f);
	setDiffuseColor(COLOR_GREEN);
	FbxNode* lRootNode = lScene->GetRootNode();
	if (lRootNode) {
		glTranslated(VAL(XPOS), VAL(YPOS), VAL(ZPOS));
		for (int i = 0; i < lRootNode->GetChildCount(); i++)
			drawNodes(lRootNode->GetChild(i));
	}
}

FBXManager::FBXManager() {
	lSdkManager = FbxManager::Create();
	lScene = NULL;
	// Create the IO settings object.
	ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ios);
	loaded = false;
	fileName = NULL;

}

bool FBXManager::drawNodes(FbxNode* pNode)
{
	glPushMatrix();
	FbxDouble3 translation = pNode->LclTranslation.Get();
	FbxDouble3 rotation = pNode->LclRotation.Get();
	FbxDouble3 scaling = pNode->LclScaling.Get();

	glTranslated(translation[0], translation[1], translation[2]);
	glRotated(rotation[0], 1, 0, 0);
	glRotated(rotation[1], 0, 1, 0);
	glRotated(rotation[2], 0, 0, 1);
	glScaled(scaling[0], scaling[1], scaling[2]);


	fbxsdk::FbxMesh* pMesh = pNode->GetMesh();

	if (pMesh) {
		int triangleCount = pMesh->GetPolygonCount();
		int vertexCounter = 0;
		double vertex[12];
		int ctrlPointIndex = 0;
		for (int i = 0; i < triangleCount; i++)
		{
			int numOfVertices = pMesh->GetPolygonSize(i);
			for (int j = 0; j < numOfVertices; j++)
			{

				ctrlPointIndex = pMesh->GetPolygonVertex(i, j);
				FbxVector4* pCtrlPoint = pMesh->GetControlPoints();
				vertex[j * 3 + 0] = pCtrlPoint[ctrlPointIndex][0];
				vertex[j * 3 + 1] = pCtrlPoint[ctrlPointIndex][1];
				vertex[j * 3 + 2] = pCtrlPoint[ctrlPointIndex][2];

			}
			//assume 3/4 edge 
			if(numOfVertices >= 3)
				drawTriangle(vertex[0], vertex[1], vertex[2], vertex[3], vertex[4], vertex[5], vertex[6], vertex[7], vertex[8]);
			if (numOfVertices >= 4) {
				drawTriangle(vertex[0], vertex[1], vertex[2], vertex[6], vertex[7], vertex[8],  vertex[9], vertex[10], vertex[11]);
			}
		}


	}

	// Draw childrens
	for (int j = 0; j < pNode->GetChildCount(); j++)
		drawNodes(pNode->GetChild(j));

	glPopMatrix();
	return false;
}



//FROM https://help.autodesk.com/view/FBX/2020/ENU/?guid=FBX_Developer_Help_getting_started_your_first_fbx_sdk_program_html