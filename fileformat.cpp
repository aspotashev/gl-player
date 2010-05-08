#include <assert.h>
#include "fileformat.h"

FileFormat::FileFormat()
{
	fd = NULL;
}

FileFormat::~FileFormat()
{
	if (fd)
	{
		closeFile();
	}
}

void FileFormat::openFile(const QString &fn)
{
	if (fd)
	{
		assert(0);
	}

	fd = new QFile(fn);
	if (!fd->open(QIODevice::ReadOnly))
	{
		assert(0);
	}

	assert(fd->read((char *)&fileHeader, sizeof(fileHeader)) ==
		sizeof(fileHeader));
	assert(fileHeader.fileVersion == 2);

	for (int i = 0; i < fileHeader.nFrames; i ++)
	{
		frameOffsets.push_back(fd->pos());

		int nEdges;
		fd->seek(fd->pos() + 4); // skip the pressure (float, 4 bytes)
		assert(fd->read((char *)&nEdges, sizeof(nEdges)) ==
			sizeof(nEdges));

		fd->seek(fd->pos() +
			fileHeader.nParticles * sizeof(VertexStruct) +
			nEdges * sizeof(EdgeStruct));
	}
}

void FileFormat::closeFile()
{
	assert(fd);

	frameOffsets.clear();
	fd->close();
}

void FileFormat::readFrame(int index,
	std::vector<VertexStruct> &v, std::vector<EdgeStruct> &e)
{
	fileSeekFrame(index);

	int nEdges;
	float pressure;
	assert(fd->read((char *)&pressure, sizeof(pressure)) ==
		sizeof(pressure));
	assert(fd->read((char *)&nEdges, sizeof(nEdges)) ==
		sizeof(nEdges));

	for (int i = 0; i < fileHeader.nParticles; i ++)
	{
		VertexStruct vertex;
		assert(fd->read((char *)&vertex, sizeof(vertex)) ==
			sizeof(vertex));
		v.push_back(vertex);
	}

	for (int i = 0; i < nEdges; i ++)
	{
		EdgeStruct edge;
		assert(fd->read((char *)&edge, sizeof(edge)) ==
			sizeof(edge));
		e.push_back(edge);
	}
}

int FileFormat::nFrames() const
{
	return fileHeader.nFrames;
}

int FileFormat::nVertices() const
{
	return fileHeader.nParticles;
}

void FileFormat::fileSeekFrame(int index)
{
	fd->seek(frameOffsets[index]);
}

std::vector<float> FileFormat::loadPressureData()
{
	std::vector<float> data;

	for (int i = 0; i < nFrames(); i ++)
	{
		fileSeekFrame(i);

		float pressure;
		assert(fd->read((char *)&pressure, sizeof(pressure)) ==
			sizeof(pressure));
		data.push_back(pressure);
	}

	return data;
}

