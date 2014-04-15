#ifndef GameTimer_h__
#define GameTimer_h__

class GameTimer{
public:
	GameTimer(void);
	~GameTimer(void);
	double elapsed, current, last;
	void Update(double wglTime);
};

#endif // GameTimer_h__
