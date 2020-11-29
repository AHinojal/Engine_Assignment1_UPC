#pragma once
class ConsoleAssimp
{
public:
	ConsoleAssimp();
	~ConsoleAssimp();

	void AddLog(char* log);
	void Draw();
private:
	bool enableScroll = true;

};

