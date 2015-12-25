#include <modapi/mod.h>

#include <engine/storage.h>
#include <engine/shared/datafile.h>
#include <engine/graphics.h>

#include <engine/external/pnglite/pnglite.h>

CModAPI_ModCreator::CModAPI_ModCreator()
{
	
}
	
//~ int CModAPI_ModCreator::AddImage(const char* pFileName)
//~ {
	//~ //Load image
	
	//~ return true;
//~ }

int CModAPI_ModCreator::AddImage(IStorage* pStorage, const char* pFilename)
{
	CModAPI_ModItem_Image Image;
	
	char aCompleteFilename[512];
	unsigned char *pBuffer;
	png_t Png; // ignore_convention

	// open file for reading
	png_init(0,0); // ignore_convention

	IOHANDLE File = pStorage->OpenFile(pFilename, IOFLAG_READ, IStorage::TYPE_ALL, aCompleteFilename, sizeof(aCompleteFilename));
	if(File)
	{
		io_close(File);
	}
	else
	{
		dbg_msg("game/png", "failed to open file. filename='%s'", pFilename);
		return -1;
	}

	int Error = png_open_file(&Png, aCompleteFilename); // ignore_convention
	if(Error != PNG_NO_ERROR)
	{
		dbg_msg("game/png", "failed to open file. filename='%s'", aCompleteFilename);
		if(Error != PNG_FILE_ERROR)
		{
			png_close_file(&Png); // ignore_convention
		}
		return -1;
	}

	if(Png.depth != 8 || (Png.color_type != PNG_TRUECOLOR && Png.color_type != PNG_TRUECOLOR_ALPHA) || Png.width > (2<<12) || Png.height > (2<<12)) // ignore_convention
	{
		dbg_msg("game/png", "invalid format. filename='%s'", aCompleteFilename);
		png_close_file(&Png); // ignore_convention
		return -1;
	}

	pBuffer = (unsigned char *)mem_alloc(Png.width * Png.height * Png.bpp, 1); // ignore_convention
	png_get_data(&Png, pBuffer); // ignore_convention
	png_close_file(&Png); // ignore_convention

	Image.m_Id = m_Images.size();
	Image.m_Width = Png.width; // ignore_convention
	Image.m_Height = Png.height; // ignore_convention
	if(Png.color_type == PNG_TRUECOLOR) // ignore_convention
		Image.m_Format = CImageInfo::FORMAT_RGB;
	else if(Png.color_type == PNG_TRUECOLOR_ALPHA) // ignore_convention
		Image.m_Format = CImageInfo::FORMAT_RGBA;
	
	m_Images.add(Image);
	m_ImagesData.add(pBuffer);
	
	return Image.m_Id;
}

int CModAPI_ModCreator::AddSprite(int ImageId, int External, int x, int y, int w, int h, int gx, int gy)
{	
	CModAPI_ModItem_Sprite sprite;
	sprite.m_Id = m_Sprites.size();
	sprite.m_X = x;
	sprite.m_Y = y;
	sprite.m_W = w;
	sprite.m_H = h;
	sprite.m_External = External;
	sprite.m_ImageId = ImageId;
	sprite.m_GridX = gx;
	sprite.m_GridY = gy;
	
	m_Sprites.add(sprite);
	
	return sprite.m_Id;
}

int CModAPI_ModCreator::AddSpriteInternal(int ImageId, int x, int y, int w, int h, int gx, int gy)
{
	if(ImageId >= MODAPI_NB_INTERNALIMG || ImageId < 0) return -1;
	return AddSprite(ImageId, 0, x, y, w, h, gx, gy);
}

int CModAPI_ModCreator::AddSpriteExternal(int ImageId, int x, int y, int w, int h, int gx, int gy)
{
	if(ImageId >= m_Images.size() || ImageId < 0) return -1;
	return AddSprite(ImageId, 1, x, y, w, h, gx, gy);
}

int CModAPI_ModCreator::Save(class IStorage *pStorage, const char *pFileName)
{
	CDataFileWriter df;
	if(!df.Open(pStorage, pFileName)) return 0;
	
	//Save images
	for(int i=0; i<m_Images.size(); i++)
	{
		CModAPI_ModItem_Image* pImage = &m_Images[i];
		int PixelSize = pImage->m_Format == CImageInfo::FORMAT_RGB ? 3 : 4;
		pImage->m_ImageData = df.AddData(pImage->m_Width*pImage->m_Height*PixelSize, m_ImagesData[i]);
		df.AddItem(MODAPI_MODITEMTYPE_IMAGE, i, sizeof(CModAPI_ModItem_Image), pImage);
	}
	
	//Save sprites
	for(int i=0; i<m_Sprites.size(); i++)
	{
		df.AddItem(MODAPI_MODITEMTYPE_SPRITE, i, sizeof(CModAPI_ModItem_Sprite), &m_Sprites[i]);
	}
	
	df.Finish();
	
	return 1;
}
