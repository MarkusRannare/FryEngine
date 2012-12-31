#include "TextureManager.h"

#include "Assert.h"
#include "Texture.h"
#include "Texture2D.h"

#include <cstring>
#include <il/il.h>

#define GLEW_STATIC 
#include <glew/glew.h>


#ifdef WIN32
	#pragma comment( lib, "DevIL.lib" )
#endif

using namespace MackyEngine;

static int				fCreateCount = 0;
static TextureManager*	fInstance = NULL;

TextureManager::TextureManager()
{
	ASSERT( fInstance == NULL );
}

TextureManager::~TextureManager()
{
	ASSERT( fInstance == NULL );
	ASSERT( fCreateCount == NULL );
}

bool TextureManager::initialize( TextureManager* manager )
{
	ASSERT( fInstance == NULL );
	ASSERT( manager != NULL );
	
	fInstance = manager;
	
	glEnable( GL_TEXTURE_2D );
	ilInit();
	
	return true;
}

void TextureManager::finalize()
{
	ASSERT( fInstance != NULL );
	ASSERT( fCreateCount == 0 );

	std::list<TextureResource>& tex = fInstance->mTextures;
	std::list<ShaderResource>& shaderTex = fInstance->mShaderTextures;
	
	for( std::list<TextureResource>::iterator it = tex.begin(); it != tex.end(); ++it )
	{
		delete[] it->filename;
		it->texture->release();		
	}
	while( shaderTex.size() > 0 )
	{
		fInstance->unload( shaderTex.begin()->texture );
		shaderTex.pop_front();
	}
	
	ilShutDown();
	
	fInstance = NULL;
}

TextureManager* TextureManager::create()
{
	ASSERT( fInstance != NULL );
	
	++fCreateCount;
	
	return fInstance;
}

void TextureManager::release()
{
	ASSERT( fInstance != NULL );
	ASSERT( fInstance == this );
	ASSERT( fCreateCount > 0 );
	
	--fCreateCount;
}

Texture2D* TextureManager::load( Texture* texture )
{
	unsigned tex;
	glGenTextures( 1, &tex );
	glBindTexture( GL_TEXTURE_2D, tex );
	
	glTexImage2D( GL_TEXTURE_2D, 0, 3, texture->getWidth(), texture->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, texture->getData() );
	
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );	// Linear Filtering
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );	// Linear Filtering
	
	glBindTexture( GL_TEXTURE_2D, 0 );
	
	Texture2D* shaderTex = new Texture2D( tex, texture->getWidth(), texture->getHeight() );
	
	ShaderResource res = { shaderTex };
	mShaderTextures.push_back( res );
	
	return shaderTex;
}

void TextureManager::unload( Texture2D* texture )
{
	glDeleteTextures( 1, (const GLuint*)texture->getData() );
}

Texture* TextureManager::load( const char* filename )
{
	Texture* texture = NULL;
	for( std::list<TextureResource>::iterator it = mTextures.begin(); it != mTextures.end(); ++it )
	{
		if( it->filename == filename )
		{
			texture = it->texture;
		}
	}
	if( texture == NULL )
	{
		unsigned imageId;
		ilGenImages( 1, &imageId );
		ilBindImage( imageId );
		if( !ilLoadImage( filename ) )
		{
			ilDeleteImages( 1, &imageId );
			ilBindImage( 0 );
			return NULL;
		}
		
		int width = ilGetInteger( IL_IMAGE_WIDTH );
		int height = ilGetInteger( IL_IMAGE_HEIGHT );
		int bpp = ilGetInteger( IL_IMAGE_BPP );
		
		ilBindImage( 0 );
		
		TextureResource res;
		texture = new Texture( width, height, bpp, imageId );
		res.texture = texture;
		// Retain \0 delimiter
		int length = strlen( filename ) + 1;
		res.filename = new char[length];
		strncpy( res.filename, filename, length );
		
		mTextures.push_back( res );
	}
	
	return texture;	
}