#pragma once
#include <iostream>
#include <thread>
#include <functional>
#include <chrono>

class CCallbackTimer
{
public:
	CCallbackTimer():bIsExcute(false)
	{};
	~CCallbackTimer() 
	{
		if (bIsExcute)	Stop();
	};
public:
	//T&& �� ������ ���۷����� �޴°� �ƴ�.
	//forward reference  ��� �Ͽ� �������� ���� ������ ���� �������� ���� ������ ���޵ȴ�.
	//T&& callback �� ���ٷ� �Ѿ�� ��� ������
	//l/r value���� Ȯ���� �����ϱ� ���� perfect forward�� ���� ���� ������ std::forward.
	//forward reference�� ���� �Լ��� ���ڷ� ���� ���� ���۷����� �̸��� �ְ� �ּҰ� ��������
	//lvalue reference�� rvalue reference�� �� ��ü�� lvalue�̴�. 
	//�� lvalue�� lvalue lvalue reference���� rvalue reference���� �������ִ� ���� std::forward.
	template<typename T, typename... Types>
	void Start(unsigned int period, T&& callback, Types&&... args)
	{
		//auto task = [callback, args...]() {callback(args...); };
		//args...ĸ�ĸ� &args�� ���۷����� �ؾ� std::forward���� ���۷����� �߷��� �� �ִ� �� ����.
		auto task = [callback, &args...](){callback(std::forward<Types>(args)...); };
		bIsExcute = true;
		workThread = std::thread(
			[this, period, task]()
			{
				while (true) 
				{
					//�׳� true�� ���� ���� ��� this ��ü�� ����� ������ ���� ���͵�
					//0�� �ƴ϶� true�� �ν��ع�����. 1�� �ƴ� �� ������ Ż���ؼ� �����尡 �������� ����.
					if (this->bIsExcute != 1) break;
					std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();
					std::chrono::system_clock::time_point nextTime = startTime + std::chrono::milliseconds(period);
					task();
					std::this_thread::sleep_until(nextTime);
					//std::cout << this << std::endl;
				}
			}
		);
		//workThread.detach();		//�ڵ��� �ٷ� ��ȯ�Ѵ�.
	}
	void Stop() 
	{
		bIsExcute = false;
		workThread.join();			//�����尡 ����Ǵ°� ��ٸ� �Ŀ� ���� ���� �����Ѵ�.
	}
public:
	bool bIsExcute;
	std::thread workThread;
};

//example
//version 1
//callable�� std::function���� ����ϰ� �ۿ��� ȣ���� ��
//std::bind�� ���ڿ� �ݹ��Լ��� ���ļ� ���� ���� �Ѱܹ޾Ƽ� �̿�
/*
void Start(unsigned int period, std::function<void()> func)
{
	bIsExcute = true;
	workThread = std::thread(
		[this, period, func]()
		{
			while (this->bIsExcute)
			{
				std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();
				std::chrono::system_clock::time_point nextTime = startTime + std::chrono::milliseconds(period);
				func();
				std::this_thread::sleep_until(nextTime);
			}
		}
	);
	workThread.detach();
}
*/
/*
void add(int x, int y)
{
	std::cout << x << " + " << y << " = " << x + y 
 std::endl;
}
*/
/*
int main() 
{
	std::function<void(int, int)> pFADD1 = std::bind(add, 2, 3);
	pFADD1(3, 5);
	//bind�� ���ڿ� ���� 3+5�� �ƴϰ� 2+3 ����.
	//�� ��� ���ڸ� ���缭 pFADD(int,int)�� ȣ������� �ϴµ�
	//callbacktimer start(int, std::function<void()>)���� ȣ���ϱ� ���ؼ�
	//auto�� �����ؼ� bind�ϰ� pFADD2() ó�� ���ڸ� ���ּ� ȣ���ؼ� ���°� ���ڴ�.
	auto pFADD2 = std::bind(add, 2, 3);
	pFADD2();
	
	CCallbackTimer calltimer;
	calltimer.Start(1000, pFADD2);
}
*/
//version 2
//�������� ���ø����� �ݹ�� ���ڸ� �޾Ƽ� �Լ� �ȿ��� ���ε�
/*
template<typename T, typename... Types>
void Start(unsigned int period, T callback, Types... args)
{
	auto task = std::bind(callback, args...);
	bIsExcute = true;
	workThread = std::thread(
		[this, period, task]()
		{
			while (this->bIsExcute)
			{
				std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();
				std::chrono::system_clock::time_point nextTime = startTime + std::chrono::milliseconds(period);
				//func();
				task();
				std::this_thread::sleep_until(nextTime);
			}
		}
	);
	workThread.detach();
}
*/
/*
void add(int x, int y)
{
	std::cout << x << " + " << y << " = " << x + y << std::endl;
}
*/
/*
int main()
{
	std::function<void(int, int)> pFADD3 = add;
    CCallbackTimer calltimer;
	//�������ڷ� �Ѱܼ� Start�Լ� �ȿ��� pFADD3 callable�� ���� ���ε�.
    calltimer.Start(1000, pFADD3, 1,2);
}
*/
//version3 (now)
//std::bind�� ������ �� �����ε�(���ڰ� �ٸ� ������ �Լ�)ȣ�� ���� ���� ����(�Լ� �����ͷ� �����ε��� �Լ��� �����ΰ� �Ѱ���� �Ѵٰ� ��)������
//���ٸ� �̿��� ���� �����Ѵٰ� �Ͽ� ���ٷ� �������ڸ� ó���� �� �ֵ���.
//�ٸ� �������ڸ� �޴� �������� ���� T&&�� ������ ���۷��� ������ �޴��� �̰� �ǹ̸� �� �𸣰ڴ�.(������ ��!)
//T&&�� ������ ���۷����� �ƴ� ���� ���۷���Forwarding Reference �Ǵ� Universal Reference
//lvalue�� lvalue, rvalue�� rvalue, const lvalue�� const lvalue, const rvalue�� const rvalue�� �߷����ش�.
//���޵Ǵ� ���� lvalue reference���� rvalue reference���� const l/r value reference����
//Ÿ���� ����� �����ϱ� ���� ���� perfect forwarding�̶�� �ϰ� �׸� ���� ���� ���۷����� std::forward�� �̿�
/*
template<typename T, typename... Types>
	void Start(unsigned int period, T&& callback, Types&&... args)
	{
		//auto task = [callback, args...]() {callback(args...); };
		//args...ĸ�ĸ� &args�� �ؾ� std::forward���� ���۷����� ����� lvalue�� ���� �� �ִ� �� ����.
		auto task = [callback, &args...](){callback(std::forward<Types>(args)...); };
		bIsExcute = true;
		workThread = std::thread(
			[this, period, task]()
			{
				while (true)
				{
					//�׳� true�� ���� ���� ��� this ��ü�� ����� ������ ���� ���͵�
					//0�� �ƴ϶� true�� �ν��ع�����. 1�� �ƴ� �� ������ Ż���ؼ� �����尡 �������� ����.
					if (this->bIsExcute != 1) break;
					std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();
					std::chrono::system_clock::time_point nextTime = startTime + std::chrono::milliseconds(period);
					task();
					std::this_thread::sleep_until(nextTime);
					//std::cout << this << std::endl;
				}
			}
		);
		workThread.detach();
	}
*/
/*
CCallbackTimer calltimer;
calltimer.Start(1000, [](int x, int y) {std::cout << x << " + " << y << " = " << x + y << std::endl; }, 4,5);
*/