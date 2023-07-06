#include "pch.h"
#include "CompressedFrame.h"

pa::CompressedFrame::CompressedFrame()
{
	auto a = sizeof(CompressedFrame);
	static_assert(10 == sizeof(CompressedFrame), "the sizeof CompressedFrame have to be 10 bytes");
}

inline pa::ChannelType pa::CompressedFrame::getChannelType() const
{
	// Most significant two bits of this 8bit index contains channel type info.
	return static_cast<ChannelType>((_jointIndex & (0xc000)) >> 14);
}

inline void pa::CompressedFrame::SetChannelType(ChannelType type)
{
	_jointIndex |= (static_cast<uint16_t>(type) << 14);
}

void pa::CompressedFrame::setJointIndex(uint16_t jointIndex)
{
	_jointIndex = jointIndex & 0x3fff;
}

inline std::uint16_t pa::CompressedFrame::getJointIndex() const
{
	return _jointIndex & 0x3fff;
}

inline uint16_t pa::CompressedFrame::getKeyTime() const
{
	return _keyTime;
}

inline void pa::CompressedFrame::setKeyTime(uint16_t keyTime)
{
	_keyTime = keyTime;
}
