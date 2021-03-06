#include <core\Bitmap.hpp>
#include <core\Timer.hpp>
#include <ImfInputFile.h>
#include <ImfOutputFile.h>
#include <ImfChannelList.h>
#include <ImfStringAttribute.h>
#include <ImfVersion.h>
#include <ImfIO.h>

NAMESPACE_BEGIN

Bitmap::Bitmap(const Vector2i & Size) : Base(Size.y(), Size.x()) { }

Bitmap::Bitmap(const std::string & Filename)
{
	Imf::InputFile File(Filename.c_str());
	const Imf::Header & Header = File.header();
	const Imf::ChannelList & Channels = Header.channels();

	Imath::Box2i DataWindow = File.header().dataWindow();
	resize(DataWindow.max.y - DataWindow.min.y + 1, DataWindow.max.x - DataWindow.min.x + 1);
	
	LOG(INFO) << "Reading a " << cols() << "x" << rows() << " OpenEXR file from \"" << Filename << "\"";
	cout.flush();
	Timer ObjTimer;

	const char * pChR = nullptr, * pChG = nullptr, * pChB = nullptr;
	for (Imf::ChannelList::ConstIterator Iter = Channels.begin(); Iter != Channels.end(); ++Iter)
	{
		std::string Name = ToLower(Iter.name());

		if (Iter.channel().xSampling != 1 || Iter.channel().ySampling != 1)
		{
			LOG(WARNING) << "Sub-sampled layers are not supported";
			continue;
		}

		if (pChR == nullptr && (Name == "r" || Name == "red" || EndsWith(Name, ".r") || EndsWith(Name, ".red")))
		{
			pChR = Iter.name();
		}
		else if (pChG == nullptr && (Name == "g" || Name == "green" || EndsWith(Name, ".g") || EndsWith(Name, ".green")))
		{
			pChG = Iter.name();
		}
		else if (pChB == nullptr && (Name == "b" || Name == "blue" || EndsWith(Name, ".b") || EndsWith(Name, ".blue")))
		{
			pChB = Iter.name();
		}
	}

	if (pChR == nullptr || pChG == nullptr || pChB == nullptr)
	{
		throw HikariException("This is not a standard RGB OpenEXR file!");
	}

	size_t CompStride = sizeof(float);
	size_t PixelStride = 3 * CompStride;
	size_t RowStride = PixelStride * cols();

	char * pPtr = (char*)(data());

	Imf::FrameBuffer FrameBuffer;
	FrameBuffer.insert(pChR, Imf::Slice(Imf::FLOAT, pPtr, PixelStride, RowStride));
	pPtr += CompStride;
	FrameBuffer.insert(pChG, Imf::Slice(Imf::FLOAT, pPtr, PixelStride, RowStride));
	pPtr += CompStride;
	FrameBuffer.insert(pChB, Imf::Slice(Imf::FLOAT, pPtr, PixelStride, RowStride));

	File.setFrameBuffer(FrameBuffer);
	File.readPixels(DataWindow.min.y, DataWindow.max.y);

	LOG(INFO) << "Done. (Took " << ObjTimer.ElapsedString() << " and " << MemString(3 * sizeof(float) * cols() * rows()) << ")";
}

void Bitmap::Save(const std::string & Filename)
{
	LOG(INFO) << "Writing a " << cols() << "x" << rows() << " OpenEXR file to \"" << Filename << "\"";
	cout.flush();
	Timer ObjTimer;

	Imf::Header Header((int)(cols()), (int)(rows()));
	Header.insert("comments", Imf::StringAttribute("Generated by Hikari"));

	Imf::ChannelList & Channels = Header.channels();
	Channels.insert("R", Imf::Channel(Imf::FLOAT));
	Channels.insert("G", Imf::Channel(Imf::FLOAT));
	Channels.insert("B", Imf::Channel(Imf::FLOAT));

	Imf::FrameBuffer FrameBuffer;
	size_t CompStride = sizeof(float);
	size_t PixelStride = 3 * CompStride;
	size_t RowStride = PixelStride * cols();

	char * pPtr = (char*)(data());
	FrameBuffer.insert("R", Imf::Slice(Imf::FLOAT, pPtr, PixelStride, RowStride));
	pPtr += CompStride;
	FrameBuffer.insert("G", Imf::Slice(Imf::FLOAT, pPtr, PixelStride, RowStride));
	pPtr += CompStride;
	FrameBuffer.insert("B", Imf::Slice(Imf::FLOAT, pPtr, PixelStride, RowStride));

	Imf::OutputFile File(Filename.c_str(), Header);
	File.setFrameBuffer(FrameBuffer);
	File.writePixels((int)(rows()));

	LOG(INFO) << "Done. (Took " << ObjTimer.ElapsedString() << ")";
}

NAMESPACE_END