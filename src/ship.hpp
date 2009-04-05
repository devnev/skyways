#ifndef _SHIP_HPP_
#define _SHIP_HPP_

class Ship
{
public:

	Ship(double maxSpeed)
		: _maxSpeed( maxSpeed ), _speed( 0 ), _xpos( 0 )
	{
	}

	void decreaseSpeed(double amount)
	{
		_speed -= amount;
		if (_speed < 0)
			_speed = 0;
	}
	void increaseSpeed(double amount)
	{
		_speed += amount;
		if (_speed > _maxSpeed)
			_speed = _maxSpeed;
	}
	double speed() const throw() { return _speed; }

	void moveLeft(double amount)
	{
		_xpos -= amount;
	}
	void moveRight(double amount)
	{
		_xpos += amount;
	}
	double xpos() const throw() { return _xpos; }

private:

	double _xpos;
	double _maxSpeed;
	double _speed;

};

#endif // _SHIP_HPP_
