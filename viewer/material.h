#pragma once

#include <array>

namespace sec21::viewer
{
struct material
{
	using presc_t = float;
	using value_t = std::array<presc_t, 3>;

	value_t ambient{};
	value_t diffuse{};
	value_t specular{};
	presc_t shininess{};
};

// material library
//! \see http://devernay.free.fr/cours/opengl/materials.html
#ifdef __cpp_designated_initializers
static constexpr auto green_plastic = material{
	.ambient{ 0.0f, 0.0f, 0.0f },
	.diffuse{ 0.1f, 0.35f, 0.1f },
	.specular{ 0.45f, 0.55f, 0.45f },
	.shininess{ 0.25f }
};
static constexpr auto ruby = material{
	.ambient{ 0.1745f, 0.01175f, 0.01175f },
	.diffuse{ 0.61424f, 0.04136f, 0.04136f },
	.specular{ 0.727811f, 0.626959f, 0.626959f },
	.shininess{ 0.6f }
};
static constexpr auto emerald = material{
	.ambient{ 0.0215f, 0.1745f, 0.0215f },
	.diffuse{ 0.07568f, 0.61424f, 0.07568f },
	.specular{ 0.633f, 0.727811f, 0.633f },
	.shininess{ 0.6f }
};
static constexpr auto jade = material{
	.ambient{ 0.135f, 0.2225f, 0.1575f },
	.diffuse{ 0.54f, 0.89f, 0.63f },
	.specular{ 0.316228f, 0.316228f, 0.316228f },
	.shininess{ 0.1f }
};
static constexpr auto chrome = material{
	.ambient{ 0.25f, 0.25f, 0.25f },
	.diffuse{ 0.4f, 0.4f, 0.4f },
	.specular{ 0.774597f, 0.774597f, 0.774597f},
	.shininess{ 0.6f }
};
#else
static constexpr auto green_plastic = material{
	{ 0.0f, 0.0f, 0.0f },
	{ 0.1f, 0.35f, 0.1f },
	{ 0.45f, 0.55f, 0.45f },
	{ 0.25f }
};
static constexpr auto ruby = material{
	{ 0.1745f, 0.01175f, 0.01175f },
	{ 0.61424f, 0.04136f, 0.04136f },
	{ 0.727811f, 0.626959f, 0.626959f },
	{ 0.6f }
};
static constexpr auto emerald = material{
	{0.0215f, 	0.1745f, 	0.0215f },
	{ 0.07568f, 	0.61424f, 	0.07568f },
	{ 0.633f, 	0.727811f, 	0.633f },
	{ 0.6f }
};
static constexpr auto jade = material{
	{ 0.135f, 0.2225f, 0.1575f },
	{ 0.54f, 0.89f, 0.63f },
	{ 0.316228f, 0.316228f, 0.316228f },
	{ 0.1f }
};
static constexpr auto chrome = material{
	{ 0.25f, 0.25f, 0.25f },
	{ 0.4f, 0.4f, 0.4f },
	{ 0.774597f, 0.774597f, 0.774597f},
	{ 0.6f }
};
static constexpr auto turquoise = material{
	{ 0.1f, 0.18725f, 0.1745f  },
	{ 0.396f, 0.74151f, 0.69102f },
	{ 0.297254f, 0.30829f, 0.306678f },
	{ 0.1f }
};
static constexpr auto obsidian = material{
	{ 0.05375f, 0.05f, 0.06625f },
	{ 0.18275f, 0.17f, 0.22525f },
	{ 0.332741f, 0.328634f, 0.346435f },
	{ 0.3f }
};
#endif   
} // sec21::viewer
