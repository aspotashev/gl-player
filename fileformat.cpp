#include <assert.h>
#include "fileformat.h"

FileFormat::FileFormat()
{
	fd = NULL;
}

FileFormat::~FileFormat()
{
}

void FileFormat::openFile(const char *fn)
{
	if (fd)
	{
		assert(0);
	}

	fd = fopen(fn, "rb");
	assert(fd);

	fread(&fileHeader, sizeof(fileHeader), 1, fd);

	for (int i = 0; i < fileHeader.nFrames; i ++)
	{
		frameOffsets.push_back(ftell(fd));

		int nEdges;
		fread(&nEdges, sizeof(nEdges), 1, fd);
		assert(fseek(fd,
			fileHeader.nParticles * sizeof(VertexStruct) +
			nEdges * sizeof(EdgeStruct),
			SEEK_CUR) == 0);
	}
}

void FileFormat::readFrame(int index,
	std::vector<VertexStruct> &v, std::vector<EdgeStruct> &e)
{
	fseek(fd, frameOffsets[index], SEEK_SET);

	int nEdges;
	fread(&nEdges, sizeof(nEdges), 1, fd);

	for (int i = 0; i < fileHeader.nParticles; i ++)
	{
		VertexStruct vertex;
		fread(&vertex, sizeof(vertex), 1, fd);
		v.push_back(vertex);
	}

	for (int i = 0; i < nEdges; i ++)
	{
		EdgeStruct edge;
		fread(&edge, sizeof(edge), 1, fd);
		e.push_back(edge);
	}
}

