#ifndef XBOX_H
#define XBOX_H

/// <summary>
/// Holds names for SFML controller buttons for ease of use.
/// </summary>
class Xbox {
public:
	~Xbox(void);

	static const float A;
	static const float B;
	static const float X;
	static const float Y;
	static const float LB;
	static const float RB;
	static const float BACK;
	static const float START;
	static const float LEFTANALOG;
	static const float RIGHTANALOG;
private:
	Xbox(void);
};

#endif
