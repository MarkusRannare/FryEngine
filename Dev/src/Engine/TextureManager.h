#ifndef MACKYENGINE_TEXTUREMANAGER_H
#define MACKYENGINE_TEXTUREMANAGER_H

#include <list>

namespace MackyEngine
{
	class Texture;
	class Texture2D;
	
	class TextureManager
	{
		public:
			TextureManager();
			~TextureManager();
		
			static bool initialize( TextureManager* manager );
			static void finalize();
			
			static TextureManager* create();
			void release();
			
			Texture* load( const char* filename );
			
			Texture2D* load( Texture* texture ); 
			void unload( Texture2D* texture );
		private:
			TextureManager& operator=( const TextureManager& rhs );
			TextureManager( const TextureManager& copy );
		private:
			struct TextureResource
			{
				char*		filename;
				Texture*	texture;
			};
			struct ShaderResource
			{
				Texture2D*	texture;	
			};
		private:
			std::list<TextureResource>		mTextures;
			std::list<ShaderResource>		mShaderTextures;
	};
}

#endif