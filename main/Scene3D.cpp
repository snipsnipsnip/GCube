/*
 * The MIT License (MIT)
 * Copyright (c) 2011 GClue, inc.
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "Scene3D.h"
#include "ApplicationController.h"
#include "Camera.h"
#include "PrimitiveObjectBuilder.h"
#include "SimpleShader.h"
#include "ColladaDataObject.h"
#include "Joint.h"

Scene3D::Scene3D(ApplicationController *controller) : IScene(controller) {
	LOGD("****Scene3D");
	// アクティブフラグをOFF
	activeflg = false;
	
	// カメラ
	camera = new Camera();
	camera->fieldOfView = 80;
	camera->loadPerspective();
	//	camera->transForm.translate(0, 0, 8);
	Vector3D eye = Vector3D(0,0,8);
	Vector3D at = Vector3D(0,0,0);
	Vector3D up = Vector3D(0,1,0);
	camera->transForm.lookAt(&eye, &at, &up);
	
	//    camera->transForm.rotate(30, 1.0f, 0, 0);
    //	camera->transForm.rotate(30, RotateDirX);
	
	fig = new Figure();
	fig2 = createBox(0.5, 0.5, 0.5);
	shader = new BoneShader();
	
	this->setup();
}

Scene3D::~Scene3D() {
	LOGD("****~Scene3D");
	DELETE(camera);
	DELETE(fig);
	DELETE(shader);
}

//////////////////////////////////////////////////////////
// IScene の実装
//////////////////////////////////////////////////////////

// セットアップ処理を行います.
void Scene3D::setup() {
	LOGD("****Scene3D::setup");
	/////// library_geometries
	// mesh-vertices
	const GLfloat posArray[] = {0,1,-1,0.1950903,0.9807853,-1,0.3826835,0.9238795,-1,0.5555703,0.8314696,-1,0.7071068,0.7071068,-1,0.8314696,0.5555702,-1,0.9238795,0.3826834,-1,0.9807853,0.1950903,-1,1,0,-1,0.9807853,-0.1950902,-1,0.9238796,-0.3826833,-1,0.8314697,-0.5555702,-1,0.7071068,-0.7071068,-1,0.5555702,-0.8314697,-1,0.3826833,-0.9238796,-1,0.1950901,-0.9807854,-1,-3.25841e-7,-1,-1,-0.1950907,-0.9807852,-1,-0.3826839,-0.9238793,-1,-0.5555707,-0.8314693,-1,-0.7071073,-0.7071064,-1,-0.83147,-0.5555697,-1,-0.9238798,-0.3826827,-1,-0.9807855,-0.1950894,-1,-1,9.65599e-7,-1,-0.9807851,0.1950913,-1,-0.9238791,0.3826845,-1,-0.8314689,0.5555713,-1,-0.7071059,0.7071077,-1,-0.5555691,0.8314704,-1,-0.3826821,0.9238801,-1,-0.1950888,0.9807856,-1,1.60536e-6,1,1,0.195092,0.980785,1,0.3826851,0.9238789,1,0.5555718,0.8314686,1,0.7071081,0.7071054,1,0.8314707,0.5555686,1,0.9238803,0.3826815,1,0.9807857,0.1950882,1,1,-2.24512e-6,1,0.980785,-0.1950921,1,0.923879,-0.3826848,1,0.831469,-0.5555711,1,0.7071063,-0.7071073,1,0.55557,-0.8314697,1,0.3826836,-0.9238795,1,0.1950908,-0.9807852,1,9.29825e-7,-1,1,-0.195089,-0.9807856,1,-0.3826819,-0.9238802,1,-0.5555685,-0.8314708,1,-0.707105,-0.7071086,1,-0.831468,-0.5555727,1,-0.9238783,-0.3826865,1,-0.9807846,-0.1950939,1,-1,-4.10476e-6,1,-0.9807862,0.1950859,1,-0.9238814,0.3826789,1,-0.8314725,0.5555658,1,-0.7071108,0.7071028,1,-0.5555753,0.8314662,1,-0.3826894,0.9238771,1,-0.195097,0.9807839,1,0,0,-1,-0.195097,0.9807839,3,-0.3826894,0.9238771,3,-0.5555753,0.8314662,3,-0.7071108,0.7071028,3,-0.8314725,0.5555658,3,-0.9238814,0.3826789,3,-0.9807862,0.1950859,3,-1,-4.10476e-6,3,-0.9807846,-0.1950939,3,-0.9238783,-0.3826865,3,-0.831468,-0.5555727,3,-0.707105,-0.7071086,3,-0.5555685,-0.8314708,3,-0.3826819,-0.9238802,3,-0.195089,-0.9807856,3,9.29825e-7,-1,3,0.1950908,-0.9807852,3,0.3826836,-0.9238795,3,0.55557,-0.8314697,3,0.7071063,-0.7071073,3,0.831469,-0.5555711,3,0.923879,-0.3826848,3,0.980785,-0.1950921,3,1,-2.24512e-6,3,0.9807857,0.1950882,3,0.9238803,0.3826815,3,0.8314707,0.5555686,3,0.7071081,0.7071054,3,0.5555718,0.8314686,3,0.3826851,0.9238789,3,0.195092,0.980785,3,1.60536e-6,1,3,1.60536e-6,1,5,0.195092,0.980785,5,0.3826851,0.9238789,5,0.5555718,0.8314686,5,0.7071081,0.7071054,5,0.8314707,0.5555686,5,0.9238803,0.3826815,5,0.9807857,0.1950882,5,1,-2.24512e-6,5,0.980785,-0.1950921,5,0.923879,-0.3826848,5,0.831469,-0.5555711,5,0.7071063,-0.7071073,5,0.55557,-0.8314697,5,0.3826836,-0.9238795,5,0.1950908,-0.9807852,5,9.29825e-7,-1,5,-0.195089,-0.9807856,5,-0.3826819,-0.9238802,5,-0.5555685,-0.8314708,5,-0.707105,-0.7071086,5,-0.831468,-0.5555727,5,-0.9238783,-0.3826865,5,-0.9807846,-0.1950939,5,-1,-4.10476e-6,5,-0.9807862,0.1950859,5,-0.9238814,0.3826789,5,-0.8314725,0.5555658,5,-0.7071108,0.7071028,5,-0.5555753,0.8314662,5,-0.3826894,0.9238771,5,-0.195097,0.9807839,5,0,0,5};
	// mesh-normals
	const GLfloat normalArray[] = {0,0,-1,0,0,-1,0,0,-1,0,0,-1,0,0,-1,0,0,-1,0,0,-1,0,0,-1,0,0,-1,0,0,-1,0,0,-1,0,0,-1,0,0,-1,0,0,-1,0,0,-1,0,0,-1,0,0,-1,0,0,-1,0,0,-1,0,0,-1,0,0,-1,0,0,-1,0,0,-1,0,0,-1,0,0,-1,0,0,-1,0,0,-1,0,0,-1,0,0,-1,0,0,-1,0,0,-1,0,0,-1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,-0.09801971,0.9951845,0,-0.09801971,0.9951845,0,-0.290291,0.9569385,0,-0.290291,0.9569385,0,-0.4714025,0.8819182,0,-0.4714025,0.8819182,0,-0.6343978,0.7730069,0,-0.6343978,0.7730069,0,-0.7730141,0.6343889,0,-0.7730141,0.6343889,0,-0.8819236,0.4713924,0,-0.8819236,0.4713924,0,-0.9569417,0.2902801,0,-0.9569417,0.2902801,0,-0.9951851,0.09801292,0,-0.9951851,0.09801292,0,-0.9951843,-0.09802079,0,-0.9951843,-0.09802079,0,-0.9569393,-0.2902883,0,-0.9569393,-0.2902883,0,-0.8819198,-0.4713994,0,-0.8819198,-0.4713994,0,-0.7730087,-0.6343955,0,-0.7730087,-0.6343955,0,-0.6343916,-0.7730119,0,-0.6343916,-0.7730119,0,-0.471395,-0.8819223,0,-0.471395,-0.8819223,0,-0.2902834,-0.9569407,0,-0.2902834,-0.9569407,0,-0.09801596,-0.9951849,0,-0.09801596,-0.9951849,0,0.09801775,-0.9951847,0,0.09801775,-0.9951847,0,0.2902852,-0.9569401,0,0.2902852,-0.9569401,0,0.4713968,-0.8819213,0,0.4713968,-0.8819213,0,0.6343929,-0.7730109,0,0.6343929,-0.7730109,0,0.7730098,-0.634394,0,0.7730098,-0.634394,0,0.8819206,-0.4713978,0,0.8819206,-0.4713978,0,0.9569398,-0.2902865,0,0.9569398,-0.2902865,0,0.9951846,-0.098019,0,0.9951846,-0.098019,0,0.995185,0.09801483,0,0.995185,0.09801483,0,0.9569409,0.2902827,0,0.9569409,0.2902827,0,0.8819223,0.471395,0,0.8819223,0.471395,0,0.7730116,0.6343919,0,0.7730116,0.6343919,0,0.6343949,0.7730092,0,0.6343949,0.7730092,0,0.4713982,0.8819204,0,0.4713982,0.8819204,0,0.2902864,0.9569398,0,0.2902864,0.9569398,0,0.09801876,0.9951846,0,0.09801876,0.9951846,0,0.09801876,0.9951846,0,0.09801876,0.9951846,0,0.2902864,0.9569398,0,0.2902864,0.9569398,0,0.4713982,0.8819204,0,0.4713982,0.8819204,0,0.6343949,0.7730092,0,0.6343949,0.7730092,0,0.7730116,0.6343919,0,0.7730116,0.6343919,0,0.8819223,0.471395,0,0.8819223,0.471395,0,0.9569409,0.2902827,0,0.9569409,0.2902827,0,0.995185,0.09801483,0,0.995185,0.09801483,0,0.9951846,-0.098019,0,0.9951846,-0.098019,0,0.9569398,-0.2902865,0,0.9569398,-0.2902865,0,0.8819206,-0.4713978,0,0.8819206,-0.4713978,0,0.7730098,-0.634394,0,0.7730098,-0.634394,0,0.6343929,-0.7730109,0,0.6343929,-0.7730109,0,0.4713968,-0.8819213,0,0.4713968,-0.8819213,0,0.2902852,-0.9569401,0,0.2902852,-0.9569401,0,0.09801775,-0.9951847,0,0.09801775,-0.9951847,0,-0.09801596,-0.9951849,0,-0.09801596,-0.9951849,0,-0.2902834,-0.9569407,0,-0.2902834,-0.9569407,0,-0.471395,-0.8819223,0,-0.471395,-0.8819223,0,-0.6343916,-0.7730119,0,-0.6343916,-0.7730119,0,-0.7730087,-0.6343955,0,-0.7730087,-0.6343955,0,-0.8819198,-0.4713994,0,-0.8819198,-0.4713994,0,-0.9569393,-0.2902883,0,-0.9569393,-0.2902883,0,-0.9951843,-0.09802079,0,-0.9951843,-0.09802079,0,-0.9951851,0.09801292,0,-0.9951851,0.09801292,0,-0.9569417,0.2902801,0,-0.9569417,0.2902801,0,-0.8819236,0.4713924,0,-0.8819236,0.4713924,0,-0.7730141,0.6343889,0,-0.7730141,0.6343889,0,-0.6343978,0.7730069,0,-0.6343978,0.7730069,0,-0.4714025,0.8819182,0,-0.4714025,0.8819182,0,-0.290291,0.9569385,0,-0.290291,0.9569385,0,-0.09801971,0.9951845,0,-0.09801971,0.9951845,0,-0.0980162,0.9951848,0,-0.09801971,0.9951845,4.26588e-7,-0.290291,0.9569385,3.90353e-7,-0.2902833,0.9569408,-3.97483e-7,-0.4714025,0.8819182,3.7874e-7,-0.4713956,0.8819218,-3.87539e-7,-0.6343978,0.7730069,3.19825e-7,-0.6343924,0.7730112,-3.53635e-7,-0.7730141,0.6343889,3.38213e-7,-0.7730096,0.6343944,-3.61784e-7,-0.8819236,0.4713924,2.81467e-7,-0.8819208,0.4713976,-2.98562e-7,-0.9569417,0.2902801,2.76062e-7,-0.95694,0.2902858,-3.07682e-7,-0.9951851,0.09801292,2.48481e-7,-0.9951846,0.0980181,-2.67302e-7,-0.9951843,-0.09802079,2.24328e-7,-0.9951848,-0.09801602,-2.48488e-7,-0.9569393,-0.2902883,1.87813e-7,-0.9569405,-0.2902842,-2.25376e-7,-0.8819198,-0.4713994,1.77142e-7,-0.8819217,-0.4713959,-2.1575e-7,-0.7730087,-0.6343955,1.75884e-7,-0.773011,-0.6343927,-1.80949e-7,-0.6343916,-0.7730119,1.42502e-7,-0.6343937,-0.7730101,-1.33944e-7,-0.471395,-0.8819223,0,-0.4713972,-0.8819211,-1.23231e-7,-0.2902834,-0.9569407,0,-0.2902851,-0.9569402,0,-0.09801596,-0.9951849,0,-0.09801751,-0.9951847,0,0.09801775,-0.9951847,0,0.09801661,-0.9951847,0,0.2902852,-0.9569401,0,0.2902848,-0.9569403,0,0.4713968,-0.8819213,0,0.4713965,-0.8819214,0,0.6343929,-0.7730109,0,0.6343934,-0.7730104,0,0.7730104,-0.6343934,0,0.7730098,-0.634394,0,0.8819214,-0.4713965,0,0.8819206,-0.4713978,0,0.9569404,-0.2902847,0,0.9569398,-0.2902865,0,0.9951847,-0.09801697,0,0.9951846,-0.098019,0,0.9951847,0.09801697,0,0.995185,0.09801483,0,0.9569402,0.290285,-1.22429e-7,0.9569409,0.2902827,0,0.8819212,0.4713968,-1.20067e-7,0.8819223,0.471395,0,0.7730106,0.6343932,0,0.7730116,0.6343919,0,0.6343933,0.7730104,0,0.6343949,0.7730092,0,0.4713967,0.8819213,0,0.4713982,0.8819204,0,0.2902849,0.9569403,0,0.2902864,0.9569398,0,0.09801697,0.9951847,0,0.09801876,0.9951846,0};
	// p
	const GLushort elementArray[] = {64,0,0,0,1,0,64,1,1,1,2,1,64,2,2,2,3,2,64,3,3,3,4,3,64,4,4,4,5,4,64,5,5,5,6,5,64,6,6,6,7,6,64,7,7,7,8,7,64,8,8,8,9,8,64,9,9,9,10,9,64,10,10,10,11,10,64,11,11,11,12,11,64,12,12,12,13,12,64,13,13,13,14,13,64,14,14,14,15,14,64,15,15,15,16,15,64,16,16,16,17,16,64,17,17,17,18,17,64,18,18,18,19,18,64,19,19,19,20,19,64,20,20,20,21,20,64,21,21,21,22,21,64,22,22,22,23,22,64,23,23,23,24,23,64,24,24,24,25,24,64,25,25,25,26,25,64,26,26,26,27,26,64,27,27,27,28,27,64,28,28,28,29,28,64,29,29,29,30,29,64,30,30,30,31,30,31,31,0,31,64,31,129,32,98,32,97,32,129,33,99,33,98,33,129,34,100,34,99,34,129,35,101,35,100,35,129,36,102,36,101,36,129,37,103,37,102,37,129,38,104,38,103,38,129,39,105,39,104,39,129,40,106,40,105,40,129,41,107,41,106,41,129,42,108,42,107,42,129,43,109,43,108,43,129,44,110,44,109,44,129,45,111,45,110,45,129,46,112,46,111,46,129,47,113,47,112,47,129,48,114,48,113,48,129,49,115,49,114,49,129,50,116,50,115,50,129,51,117,51,116,51,129,52,118,52,117,52,129,53,119,53,118,53,129,54,120,54,119,54,129,55,121,55,120,55,129,56,122,56,121,56,129,57,123,57,122,57,129,58,124,58,123,58,129,59,125,59,124,59,129,60,126,60,125,60,129,61,127,61,126,61,129,62,128,62,127,62,129,63,97,63,128,63,96,64,65,64,128,64,96,65,128,65,97,65,65,66,66,66,127,66,65,67,127,67,128,67,66,68,67,68,126,68,66,69,126,69,127,69,67,70,68,70,125,70,67,71,125,71,126,71,68,72,69,72,124,72,68,73,124,73,125,73,69,74,70,74,123,74,69,75,123,75,124,75,70,76,71,76,122,76,70,77,122,77,123,77,71,78,72,78,121,78,71,79,121,79,122,79,72,80,73,80,120,80,72,81,120,81,121,81,73,82,74,82,119,82,73,83,119,83,120,83,74,84,75,84,118,84,74,85,118,85,119,85,75,86,76,86,117,86,75,87,117,87,118,87,76,88,77,88,116,88,76,89,116,89,117,89,77,90,78,90,115,90,77,91,115,91,116,91,78,92,79,92,114,92,78,93,114,93,115,93,79,94,80,94,113,94,79,95,113,95,114,95,80,96,81,96,112,96,80,97,112,97,113,97,81,98,82,98,111,98,81,99,111,99,112,99,82,100,83,100,110,100,82,101,110,101,111,101,83,102,84,102,109,102,83,103,109,103,110,103,84,104,85,104,108,104,84,105,108,105,109,105,85,106,86,106,107,106,85,107,107,107,108,107,86,108,87,108,106,108,86,109,106,109,107,109,87,110,88,110,105,110,87,111,105,111,106,111,88,112,89,112,104,112,88,113,104,113,105,113,89,114,90,114,103,114,89,115,103,115,104,115,90,116,91,116,102,116,90,117,102,117,103,117,91,118,92,118,101,118,91,119,101,119,102,119,92,120,93,120,100,120,92,121,100,121,101,121,93,122,94,122,99,122,93,123,99,123,100,123,94,124,95,124,98,124,94,125,98,125,99,125,95,126,96,126,97,126,95,127,97,127,98,127,33,128,32,128,96,128,33,129,96,129,95,129,34,130,33,130,95,130,34,131,95,131,94,131,35,132,34,132,94,132,35,133,94,133,93,133,36,134,35,134,93,134,36,135,93,135,92,135,37,136,36,136,92,136,37,137,92,137,91,137,38,138,37,138,91,138,38,139,91,139,90,139,39,140,38,140,90,140,39,141,90,141,89,141,40,142,39,142,89,142,40,143,89,143,88,143,41,144,40,144,88,144,41,145,88,145,87,145,42,146,41,146,87,146,42,147,87,147,86,147,43,148,42,148,86,148,43,149,86,149,85,149,44,150,43,150,85,150,44,151,85,151,84,151,45,152,44,152,84,152,45,153,84,153,83,153,46,154,45,154,83,154,46,155,83,155,82,155,47,156,46,156,82,156,47,157,82,157,81,157,48,158,47,158,81,158,48,159,81,159,80,159,49,160,48,160,80,160,49,161,80,161,79,161,50,162,49,162,79,162,50,163,79,163,78,163,51,164,50,164,78,164,51,165,78,165,77,165,52,166,51,166,77,166,52,167,77,167,76,167,53,168,52,168,76,168,53,169,76,169,75,169,54,170,53,170,75,170,54,171,75,171,74,171,55,172,54,172,74,172,55,173,74,173,73,173,56,174,55,174,73,174,56,175,73,175,72,175,57,176,56,176,72,176,57,177,72,177,71,177,58,178,57,178,71,178,58,179,71,179,70,179,59,180,58,180,70,180,59,181,70,181,69,181,60,182,59,182,69,182,60,183,69,183,68,183,61,184,60,184,68,184,61,185,68,185,67,185,62,186,61,186,67,186,62,187,67,187,66,187,63,188,62,188,66,188,63,189,66,189,65,189,32,190,63,190,65,190,32,191,65,191,96,191,32,192,0,192,31,192,32,193,31,193,63,193,30,194,62,194,63,194,30,195,63,195,31,195,29,196,61,196,62,196,29,197,62,197,30,197,28,198,60,198,61,198,28,199,61,199,29,199,27,200,59,200,60,200,27,201,60,201,28,201,26,202,58,202,59,202,26,203,59,203,27,203,25,204,57,204,58,204,25,205,58,205,26,205,24,206,56,206,57,206,24,207,57,207,25,207,23,208,55,208,56,208,23,209,56,209,24,209,22,210,54,210,55,210,22,211,55,211,23,211,21,212,53,212,54,212,21,213,54,213,22,213,20,214,52,214,53,214,20,215,53,215,21,215,19,216,51,216,52,216,19,217,52,217,20,217,18,218,50,218,51,218,18,219,51,219,19,219,17,220,49,220,50,220,17,221,50,221,18,221,16,222,48,222,49,222,16,223,49,223,17,223,15,224,47,224,48,224,15,225,48,225,16,225,14,226,46,226,47,226,14,227,47,227,15,227,13,228,45,228,46,228,13,229,46,229,14,229,12,230,44,230,45,230,12,231,45,231,13,231,11,232,43,232,12,232,43,233,44,233,12,233,10,234,42,234,11,234,42,235,43,235,11,235,9,236,41,236,10,236,41,237,42,237,10,237,8,238,40,238,9,238,40,239,41,239,9,239,7,240,39,240,8,240,39,241,40,241,8,241,6,242,38,242,7,242,38,243,39,243,7,243,5,244,37,244,6,244,37,245,38,245,6,245,4,246,36,246,5,246,36,247,37,247,5,247,3,248,35,248,4,248,35,249,36,249,4,249,2,250,34,250,3,250,34,251,35,251,3,251,1,252,33,252,2,252,33,253,34,253,2,253,0,254,32,254,1,254,32,255,33,255,1,255};
	
	/////// library_controllers
	// vertex_weights
	const float weightArray[] = {0.03432673,0.03432703,0.0343272,0.03432732,0.03432744,0.03432756,0.03432756,0.0343275,0.03432744,0.03432732,0.0343272,0.03432708,0.03432697,0.03432685,0.03432673,0.03432661,0.03432649,0.03432631,0.03432619,0.03432601,0.03432583,0.03432571,0.03432571,0.03432577,0.03432583,0.03432589,0.03432601,0.03432607,0.03432613,0.03432619,0.03432625,0.03432643,0.4726171,0.003650009,0.4726176,0.003642022,0.4726176,0.003630638,0.4726175,0.00361526,0.4726174,0.003595352,0.472617,0.003570973,0.4726166,0.003542661,0.4726161,0.003512263,0.4726158,0.003482937,0.4726157,0.003458738,0.4726154,0.003443956,0.472615,0.003441452,0.4726147,0.003451704,0.4726145,0.003473401,0.4726144,0.003503739,0.4726145,0.003539383,0.4726147,0.003576278,0.4726147,0.003610432,0.4726148,0.003638505,0.4726147,0.003658413,0.4726146,0.003669738,0.4726145,0.003673791,0.4726146,0.003673076,0.4726148,0.003670275,0.472615,0.003667235,0.4726152,0.003664791,0.4726154,0.003663241,0.4726154,0.003662288,0.4726154,0.003661513,0.4726154,0.00366044,0.4726157,0.003658592,0.4726163,0.003655314,0.002726972,0.9184607,0.002726912,0.9184853,0.002726793,0.9184929,0.002726674,0.9184901,0.002726554,0.9184844,0.002726435,0.9184836,0.002726316,0.9184974,0.002726256,0.9185352,0.002726316,0.918603,0.002726376,0.9186956,0.002726495,0.9187898,0.002726554,0.9188163,0.002726614,0.9187093,0.002726614,0.918437,0.002726554,0.9180058,0.002726495,0.9174574,0.002726316,0.9168512,0.002726197,0.9162641,0.002726078,0.9157651,0.002726018,0.9153969,0.002725958,0.9152163,0.002725958,0.9152664,0.002726078,0.915548,0.002726256,0.9159943,0.002726376,0.9165053,0.002726495,0.9169995,0.002726614,0.9174221,0.002726793,0.9177587,0.002726912,0.918014,0.002726972,0.9181992,0.002727031,0.9183276,0.002727031,0.918411,0.7835414,0.7833193,0.7828264,0.7819342,0.7804381,0.7780166,0.7741702,0.7681253,0.7608916,0.7534752,0.7486422,0.7482426,0.7513507,0.7564724,0.7620231,0.7693182,0.7770521,0.7836803,0.7886833,0.7910952,0.7906695,0.7878352,0.7843507,0.7827178,0.7820433,0.7819883,0.7822272,0.7825749,0.7829292,0.7832341,0.7834569,0.7835708,1};
	const int jointCount[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	const int jointIndex[] = {0,0,0,1,0,2,0,3,0,4,0,5,0,6,0,7,0,8,0,9,0,10,0,11,0,12,0,13,0,14,0,15,0,16,0,17,0,18,0,19,0,20,0,21,0,22,0,23,0,24,0,25,0,26,0,27,0,28,0,29,0,30,0,31,0,32,1,33,0,34,1,35,0,36,1,37,0,38,1,39,0,40,1,41,0,42,1,43,0,44,1,45,0,46,1,47,0,48,1,49,0,50,1,51,0,52,1,53,0,54,1,55,0,56,1,57,0,58,1,59,0,60,1,61,0,62,1,63,0,64,1,65,0,66,1,67,0,68,1,69,0,70,1,71,0,72,1,73,0,74,1,75,0,76,1,77,0,78,1,79,0,80,1,81,0,82,1,83,0,84,1,85,0,86,1,87,0,88,1,89,0,90,1,91,0,92,1,93,0,94,1,95,0,96,1,97,0,98,1,99,0,100,1,101,0,102,1,103,0,104,1,105,0,106,1,107,0,108,1,109,0,110,1,111,0,112,1,113,0,114,1,115,0,116,1,117,0,118,1,119,0,120,1,121,0,122,1,123,0,124,1,125,0,126,1,127,0,128,1,129,0,130,1,131,0,132,1,133,0,134,1,135,0,136,1,137,0,138,1,139,0,140,1,141,0,142,1,143,0,144,1,145,0,146,1,147,0,148,1,149,0,150,1,151,0,152,1,153,0,154,1,155,0,156,1,157,0,158,1,159,1,160,1,161,1,162,1,163,1,164,1,165,1,166,1,167,1,168,1,169,1,170,1,171,1,172,1,173,1,174,1,175,1,176,1,177,1,178,1,179,1,180,1,181,1,182,1,183,1,184,1,185,1,186,1,187,1,188,1,189,1,190,1,191,1,192};
	
	/////////// joints
	// INV_BIND_MATRIX
	const float invArray[] = {1,0,0,0,0,0,1,-2,0,-1,0,0,0,0,0,1,1,0,0,0,0,0,1,-3,0,-1,0,0,0,0,0,1};
	// joints-array
	const std::string jointsIdArray[] = {"Bone_001", "Bone_002"};
	
	// jointデータ設定
	ColladaJointData jointData = {
        std::string("Bone"), // sid
        {0, 0, 0}, // location
        {90, 0, 0}, // rotation
        {1, 1, 1} // scale
	};
	
	ColladaJointData jointDataChild1 = {
        std::string("Bone_001"), // sid
        {0, 2, 0}, // location
        {0, 0, 0}, // rotation
        {1, 1, 1} // scale
	};
	ColladaJointData jointDataChild2 = {
        std::string("Bone_002"), // sid
        {0, 1, 0}, // location
        {0, 0, 0}, // rotation
        {1, 1, 1} // scale
	};
	jointDataChild1.childrenArray.push_back(jointDataChild2);
	jointData.childrenArray.push_back(jointDataChild1);
	
	
	// データ設定
	ColladaDataObject data;
	data.positionsArray.assign(posArray, posArray + (sizeof(posArray) / sizeof(posArray[0])));
	data.normalsArray.assign(normalArray, normalArray + (sizeof(normalArray) / sizeof(normalArray[0])));
	data.meshIndexesArray.assign(elementArray, elementArray + (sizeof(elementArray) / sizeof(elementArray[0])));
	data.weightsArray.assign(weightArray, weightArray + (sizeof(weightArray) / sizeof(weightArray[0])));
	data.weightCountsArray.assign(jointCount, jointCount + (sizeof(jointCount) / sizeof(jointCount[0])));
	data.weightIndexesArray.assign(jointIndex, jointIndex + (sizeof(jointIndex) / sizeof(jointIndex[0])));
	data.bindPosesArray.assign(invArray, invArray + (sizeof(invArray) / sizeof(invArray[0])));
	data.joint = jointData;
	data.jointsIdArray.assign(jointsIdArray, jointsIdArray + (sizeof(jointsIdArray) / sizeof(jointsIdArray[0])));
	
	// Figure作成
	fig = data.makeFigure();
	fig->build();
	fig->transForm->rotate(-90, RotateDirX);
	//	fig->transForm->rotate(20, RotateDirY);
	
	fig2->build();
	fig2->transForm->loadIdentity();
	fig2->transForm->translate(1.5,0,0);
}

// リサイズ
void Scene3D::resize(int width, int height) {
	LOGD("****Scene3D::resize:%d-%d", width, height);
	float aspect = width / (float)height;
	camera->aspect = aspect;
	camera->loadPerspective();
	Vector3D eye = Vector3D(0,2,-8);
	Vector3D at = Vector3D(0,0,0);
	//	Vector3D up = Vector3D(1,0,0);
	camera->transForm.lookAt(&eye, &at);
}

static float rot = 0;

// ステップ実行します
void Scene3D::step(float dt) {
	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	rot += 0.2;
	if (rot>30) rot = 0;
	
	shader->useProgram();
	int e = glGetError();
	if (e > 0) {
		LOGE("glGetError:1:%d", e);
	}
	fig->bind();
	e = glGetError();
	if (e > 0) {
		LOGE("glGetError:2:%d", e);
	}
	
	// ジョイントのアニメーション
    //	fig->transForm->loadIdentity();
    //	fig->transForm->rotate(rot, RotateDirY);
	Joint *joint = fig->joint;
    //	joint->transForm->loadIdentity();
    //	joint->transForm->rotate(rot, RotateDirZ);
    //	joint->getChildren()[0]->transForm->loadIdentity();
    //	joint->getChildren()[0]->transForm->rotate(rot, RotateDirZ);
	joint->getChildren()[0]->getChildren()[0]->transForm->loadIdentity();
	joint->getChildren()[0]->getChildren()[0]->transForm->rotate(rot, RotateDirZ);
    //	joint->getChildren()[1]->transForm->loadIdentity();
    //	joint->getChildren()[1]->transForm->rotate(-rot, RotateDirZ);
	joint->setSkinningMatrix(shader);
	e = glGetError();
	if (e > 0) {
		LOGE("glGetError:3:%d", e);
	}
	
	// シェーダーに値を設定
	shader->setNormalMatrix(fig->transForm);
	e = glGetError();
	if (e > 0) {
		LOGE("glGetError:4:%d", e);
	}
	shader->setMVPMatrix(camera, fig->transForm);
	e = glGetError();
	if (e > 0) {
		LOGE("glGetError:5:%d", e);
	}
	fig->draw();
	e = glGetError();
	if (e > 0) {
		LOGE("glGetError:6:%d", e);
	}
	
	shader->setMVPMatrix(camera, fig2->transForm);
	fig2->bind();
	fig2->draw();
}

// 活性化します.
void Scene3D::onActivate() {
	LOGD("****Scene3D::onActivate");
	activeflg = true;
}

// 休止します.
void Scene3D::onSuspend() {
	LOGD("****Scene3D::onSuspend");
	activeflg = false;
}

// 活性化してシーンが切り替え終わったこと通知します.
void Scene3D::onStart() {
	
}

// 非活性化してシーンが切り替え終わったこと通知します.
void Scene3D::onEnd() {
}

// コンテキストが切り替わったことを通知します.
void Scene3D::onContextChanged() {
	LOGD("****Scene3D::onContextChanged");
//	this->setup();
	fig->destroy();
	fig2->destroy();
	fig->build();
	fig2->build();
//	fig = new Figure();
//	fig2 = createBox(0.5, 0.5, 0.5);
	shader = new BoneShader();
}

// バックキーイベント
bool Scene3D::onPressBackKey() {
	LOGD("****Scene3D::onPressBackKey");
	return false;
}

// タッチイベント
void Scene3D::onTouch(TouchEvent &event) {
	LOGD("****Scene3D::onTouch:%.1f-%.1f", event.x, event.y);
}

// 加速度センサー
void Scene3D::onMoveSensor(double sensor) {
}

// JNIイベント
void Scene3D::onGameEvent(int type, int param1, int param2, int param3, int param4, const char *param5) {
	LOGI("** Scene3D::onGameEvent: %d,%d,%d,%d,%d,%s",type, param1, param2, param3, param4, param5);
}
