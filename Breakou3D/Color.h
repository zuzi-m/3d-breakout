#pragma once

#include "Vector.h"

/**
 * @class	Color
 *
 * @brief	A simple representation of RGBA color.
 * 			Has capabilities to be used with OpenGL.
 */
class Color
{
public:

	/**
	 * @fn	Color::Color();
	 *
	 * @brief	Default constructor creating a black, solid color (0,0,0,1).
	 */
	Color();

	/**
	 * @fn	Color::Color(double grayscale);
	 *
	 * @brief	Creates a grayscale color with alpha equal to 1.0.
	 *
	 * @param	grayscale	The grayscale value of the color.
	 */
	Color(double grayscale);

	/**
	 * @fn	Color::Color(double red, double green, double blue);
	 *
	 * @brief	Creates a color setting its R,G,B components and alpha to 1.0.
	 *
	 * @param	red  	The red component of the color.
	 * @param	green	The green component of the color.
	 * @param	blue 	The blue component of the color.
	 */
	Color(double red, double green, double blue);

	/**
	 * @fn	Color::Color(double red, double green, double blue, double alpha);
	 *
	 * @brief	Creates a color setting its R,G,B and alpha values directly.
	 *
	 * @param	red  	The red component of the color.
	 * @param	green	The green component of the color.
	 * @param	blue 	The blue component of the color.
	 * @param	alpha	The alpha value of the color.
	 */
	Color(double red, double green, double blue, double alpha);

	/**
	 * @fn	Color::Color(MathLib::Vector& v);
	 *
	 * @brief	Creates an RGB color from the components of given vector. Alpha will be set to 1.0.
	 *
	 * @param [in]	v	A vector whose components will make up the color.
	 */
	Color(MathLib::Vector& v);

	~Color();

	/**
	 * @fn	static Color Color::randomColor(double alpha);
	 *
	 * @brief	Creates a random color.
	 *
	 * @param	alpha	The alpha value of the color to be created.
	 *
	 * @return	A new color with random values of R,G,B and the alpha value provided.
	 */
	static Color randomColor(double alpha);

	/**
	 * @fn	static Color Color::hsv(int h, double s, double v, double alpha);
	 *
	 * @brief	Creates an RGB color from an HSV color values. This is a convenience
	 * 			method to create colors with specific hue.
	 *
	 * @param	h	 	The hue of the color in degrees of 0 to 360.
	 * @param	s	 	The saturation value of the color, in range 0.0 to 1.0.
	 * @param	v	 	The "value" (brightness) of the color, in range 0.0 to 1.0.
	 * @param	alpha	The alpha value of the color to be created.
	 *
	 * @return	An RGB Color converted from given HSV color model values.
	 */
	static Color hsv(int h, double s, double v, double alpha);

	/**
	 * @fn	void Color::bind() const;
	 *
	 * @brief	Binds the color to OpenGL context in order to start drawing with this color.
	 */
	void bind() const;

	/**
	 * @fn	double Color::operator[](const int) const;
	 *
	 * @brief	Array indexer operator returning the component of the color.
	 * 			There are 4 components in following order: R, G, B, A.
	 *
	 * @param	int	The index of the component.
	 * 				The "modulo 4" operator is applied to it before indexing.
	 *
	 * @return	The value of the color component on given index.
	 */
	double operator[](const int) const;

	/**
	 * @fn	friend std::ostream& Color::operator<<(std::ostream&, const Color&);
	 *
	 * @brief	Stream insertion operator. The function will insert each component to the given
	 * 			stream in their defined order and separates them by spaces ("R G B A").
	 *
	 * @param [in,out]	parameter1	The first parameter.
	 * @param 		  	parameter2	The second parameter.
	 *
	 * @return	The shifted result.
	 */
	friend std::ostream& operator<<(std::ostream&, const Color&);

	/**
	 * @fn	void Color::toArray(double* destArray, bool includeAlpha) const;
	 *
	 * @brief	Convert this object into an array representation. Convenience method to fill given
	 * 			array with values of this color. Usable for OpenGL's functions accepting parameters
	 * 			in an array. NOTE: does no bound checking, so the destination array must be allocated.
	 *
	 * @param [in,out]	destArray   	Array to fill with components of the color. Has to have enough
	 * 					                space for all the components of the color.
	 * @param 		  	includeAlpha	Flag indicating whether include also the alpha component.
	 */
	void toArray(double* destArray, bool includeAlpha) const;

	/**
	 * @fn	void Color::toArray(float* destArray, bool includeAlpha) const;
	 *
	 * @brief	Float version of the {@link #Color::toArray(double* destArray, bool includeAlpha) double} version
	 * 			of this method.
	 */
	void toArray(float* destArray, bool includeAlpha) const;
private:

	/**
	 * @fn	void Color::clamp();
	 *
	 * @brief	Clamps the color component into the range from 0.0 to 1.0.
	 */
	void clamp();

	double r;
	double g;
	double b;
	double a;
};

