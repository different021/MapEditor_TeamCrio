#pragma once

class Synchronization
{
private:
	HANDLE hEvent;
public:
	Synchronization();
	~Synchronization();

	void Stop();
	void Go();
	void WaitPoint();
	void End();
};

