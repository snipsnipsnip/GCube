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

#ifndef GL_TEXTURE_H
#define GL_TEXTURE_H

#include "glcommon.h"
#include "GCObject.h"
#include <string>

/**
 * テクスチャ.
 */
class Texture : public GCObject {
public:
	std::string filename;	//!< ファイル名
	GLuint texName;			//!< バインドしたときの名前.
	int width;				//!< テクスチャの横幅.
	int height;				//!< テクスチャの縦幅.

	/**
	 * コンストラクタ.
	 *
	 * 指定されたファイルからテクスチャを作成します.
	 *
	 * @param[in] fname ファイル名
	 */
	Texture(const char *fname);

	Texture();

	/**
	 * デストラクタ.
	 */
	virtual ~Texture();

	/**
	 * イメージデータを設定します.
	 * <br><br>
	 *
	 * 特にNULLチェックなどは行っていないので、変な値を入れないように。
	 * <br>
	 *
	 * @param [in] imageData イメージデータ(RGBAの生データ)
	 * @param [in] width イメージデータの横幅
	 * @param [in] height イメージデータの縦幅
	 */
	void setImageData(unsigned char *imageData, int width, int height);

	void clamp();

	virtual void reload();
};

#endif // end of GL_TEXTURE_H
