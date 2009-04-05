#ifndef _SHIP_HPP_
#define _SHIP_HPP_

class Ship
{
public:

	Ship(double maxSpeed);

	void decreaseSpeed(double amount);
	void increaseSpeed(double amount);
	double speed() const throw() { return _speed; }

	void moveLeft(double amount);
	void moveRight(double amount);
	double xpos() const throw() { return _xpos; }
	double ypos() const throw() { return _ypos; }
	double zpos() const throw() { return _zpos; }

	void jump(double strength);

	void update(double multiplier);

	void glDraw();

private:

	double _maxSpeed;
	double _speed;
	double _xpos, _ypos, _zpos;
	double _jumptimed, _jumpstrength;

};

#endif // _SHIP_HPP_
