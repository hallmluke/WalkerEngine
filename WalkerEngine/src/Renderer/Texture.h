#pragma once

#include <string>
#include <memory>

namespace Walker {

	enum class TextureFormat
	{
		None = 0,

		// Color
		RGBA8,
		RGBA16F,
		RGBA32F,
		RED_INTEGER,

		// Depth/stencil
		DEPTH24STENCIL8,
		DEPTH32F,

		// Defaults
		Depth = DEPTH32F
	};

	enum class TextureFilterType {
		None = 0,
		NEAREST,
		LINEAR,
		NEAREST_MIPMAP_NEAREST,
		LINEAR_MIPMAP_NEAREST,
		NEAREST_MIMAP_LINEAR,
		LINEAR_MIPMAP_LINEAR
	};

	enum class TextureWrapType {
		None = 0,
		CLAMP_EDGE,
		CLAMP_BORDER,
		MIRRORED_REPEAT,
		REPEAT,
		MIRROR_CLAMP_EDGE
	};

	enum class TextureType
	{
		None = 0,
		UNSIGNED_BYTE,
		UNSIGNED_INT_24_8,
		FLOAT
	};

	enum class TextureAccess
	{
		None = 0,
		READ_ONLY,
		WRITE_ONLY,
		READ_WRITE
	};

	struct TextureSpecification
	{
		TextureSpecification() = default;
		TextureSpecification(uint32_t width, uint32_t height, TextureFormat format, TextureType type, TextureWrapType wrapS, TextureWrapType wrapT,
			TextureFilterType magFilter, TextureFilterType minFilter)
			: Width(width), Height(height), TextureFormat(format), Type(type), WrapS(wrapS), WrapT(wrapT), MagFilter(magFilter), MinFilter(minFilter) {}

		uint32_t Width = 0, Height = 0, Depth = 0;
		TextureFormat TextureFormat = TextureFormat::None;
		TextureType Type = TextureType::None;
		TextureWrapType WrapS = TextureWrapType::None;
		TextureWrapType WrapT = TextureWrapType::None;
		TextureWrapType WrapR = TextureWrapType::None;
		TextureFilterType MagFilter = TextureFilterType::None;
		TextureFilterType MinFilter = TextureFilterType::None;
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void BindImage(uint32_t slot = 0, TextureAccess access = TextureAccess::READ_WRITE) const = 0;

		virtual bool IsLoaded() const = 0;
		virtual void GenerateMipMaps() = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static std::shared_ptr<Texture2D> Create(uint32_t width, uint32_t height);
		static std::shared_ptr<Texture2D> Create(const std::string& path);
		static std::shared_ptr<Texture2D> Create(const TextureSpecification& spec);
	};

	class TextureCubeMap : public Texture
	{
	public:
		static std::shared_ptr<TextureCubeMap> Create(const std::string& dir);
	};

	class Texture3D : public Texture
	{
	public:
		virtual uint32_t GetDepth() const = 0;
		static std::shared_ptr<Texture3D> Create(uint32_t width, uint32_t height, uint32_t depth);
		static std::shared_ptr<Texture3D> Create(const std::string& path);
		static std::shared_ptr<Texture3D> Create(const TextureSpecification& spec);
	};

}