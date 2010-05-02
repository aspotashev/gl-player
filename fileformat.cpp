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

	assert(fread(&fileHeader, sizeof(fileHeader), 1, fd) == 1);

	for (int i = 0; i < fileHeader.nFrames; i ++)
	{
		frameOffsets.push_back(ftell(fd));

		int nEdges;
		assert(fread(&nEdges, sizeof(nEdges), 1, fd) == 1);
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
	assert(fread(&nEdges, sizeof(nEdges), 1, fd) == 1);

	for (int i = 0; i < fileHeader.nParticles; i ++)
	{
		VertexStruct vertex;
		assert(fread(&vertex, sizeof(vertex), 1, fd) == 1);
		v.push_back(vertex);
	}

	for (int i = 0; i < nEdges; i ++)
	{
		EdgeStruct edge;
		assert(fread(&edge, sizeof(edge), 1, fd) == 1);
		e.push_back(edge);
	}
}

int FileFormat::nFrames() const
{
	return fileHeader.nFrames;
}

