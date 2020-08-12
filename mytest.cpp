///////////////////////////////////////////////////////////////////////////////
//
//		Rouguelike
//
///////////////////////////////////////////////////////////////////////////////
/*
�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
���}�b�v��\��												
���}�b�v�����ړ��ł��邱��									
���K�i�Ń}�b�v���K�w���ړ��ł��邱��				
��Gold���E�����Ƃ��ł��邱�Ɓi�X�e�[�^�X��Gold��������̂݁j
���X�e�[�^�X�\���iGold�̂݁j								
���X�e�[�^�X�\���iHP�j										
�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[



		 */
		 ///////////////////////////////////////////////////////////////////////////////
		 //	@		Rogue. ���̃V���{���͌N�A�`���҂������Ă���B
		 //	- |		Wall. �����͕����̕ǂ������Ă���B
		 //	+		Door. ��������/�ւ̃h�A�B
		 //	.		The floor of a room.
		 //	#		Floor. �����̏��B(�L��)
		 //	*		Gold. ����̎R�A��B
		 //	%		Staircase. ���̊K�ւ̊K�i�B�u > + ENTER �v�łP�K����Q�K��
		 //	:		Food. �H���B
		 ///////////////////////////////////////////////////////////////////////////////
		 // �X�N���[���̍ŉ��s(���݂̏�Ԃ�����)
		 //	Level
		 //	������K
		 //	Gold
		 //	�W�߂�����̐��B
		 //	Hp
		 //	�x�e���鎖�ŁA���N�|�C���g�͉񕜂��鎖���ł���B
		 //	���ʓ��̐��l�́A�ő�l�B
		 //	Str
		 //�����i�������j
		 //	Arm
		 //	����́i�������j
		 //	Exp
		 //�o���l�i�������j
		 //////////////////////////////////////////////////////////////////////////////////////////////////////
		 // �R�}���h+ENTER
		 // h	���ֈړ�
		 // j	���ֈړ�
		 // k	��ֈړ�
		 // l	�E�ֈړ�
		 // .	�������Ȃ��i�x�e�F�̗͂̉񕜁j
		 // >	go down a staircase. ���̊K�ւ̊K�i�������B�K�i�̏��ɗ����Ă��鎞��������鎖�͂ł��Ȃ��B
		 // Q	quit. �I���B
		 //////////////////////////////////////////////////////////////////////////
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <conio.h>

using	namespace	std;

class Player {
private:
	int	x;
	int	y;
	int	symbol;
	int	gold;
	int	amulet;
	int foods;
	int stomach;
	int	hp;
	int	maxhp;
	int	usehp;
	string	msg;
	bool	dead;
public:
	Player(int symbol)
	{
		stomach = 1000;
		foods = 0;
		amulet = 0;
		gold = 0;
		x = 0;
		y = 0;
		hp = 14;
		maxhp = 14;
		this->symbol = symbol;
		dead = false;
		usehp = 0;
	}
	void	rest()
	{
		if (usehp>0)
			usehp--;
	}
	void	useHp()
	{
		usehp++;
		if (usehp > 30)
		{	//30��ȏ�ړ������hp��1����
			usehp = 0;
			hp--;
			if (hp <= 0) {
				dead = true;
			}
		}
	}
	bool	isDead()
	{
		return	dead;
	}
	void	eatFood()
	{
		if (foods > 0) {
			foods--;
			stomach += 200;
		}
		else {
			msg = "�H�����Ȃ�";
		}
	}
	void	addFood()
	{
		foods++;
	}
	/*void	addAmulet()
	{
		amulet++;
	}*/
	/*bool	hasAmulet()
	{
		if (amulet > 0) {
			return	true;
		}
		return	false;
	}*/
	void	addGold()
	{
		gold++;
	}
	void	setPosi(int x, int y) {
		this->x = x;
		this->y = y;
	}
	void	setMessage(const char *msg) {
		this->msg = msg;
	}
	int	getX() {
		return x;
	}
	int	getY() {
		return y;
	}
	int	getSymbol() {
		return symbol;
	}
	void move(int addX, int addY) {
		x += addX;
		y += addY;
		stomach--;
		if (stomach <= 0) {
			dead = true;
		}
		useHp();
	}
	void disp(int floorNumber)
	{
		char	buf[128];
		sprintf(buf, "�K:%2d ����:%4d �̗�:%2d(%2d) ����: %2d(%2d) ���: %2d(%2d) �o��: %2d(%2d) ",
			floorNumber,
			gold,
			hp,
			maxhp,
			0,
			0,
			0,
			0,
			0,
			0);
		cout << buf;
		if (stomach < 50) {
			cout << "�Q��" << endl;
		}
		else if (stomach < 100) {
			cout << "����" << endl;
		}
		else {
			cout << endl;
		}
		cout << msg << endl;
		msg = "";
	}
};


class Floor {
private:
	vector<string> map;
	int		player_x;
	int		player_y;
public:
	Floor()
	{
		player_x = 0;
		player_y = 0;
	}
	void	setPlayerPosi(Player & player)
	{
		player.setPosi(player_x, player_y);
	}
	bool	load(const char *fname)
	{
		std::ifstream ifs(fname);
		if (!ifs) {
			return	false;
		}
		std::string buf;
		int		i = 0;
		while (getline(ifs, buf)) {
			int pos = buf.find("@");
			if (pos != string::npos)
			{
				buf[pos] = '.';
				player_x = pos;
				player_y = i;
			}
			map.push_back(buf);
			i++;
		}
		return	true;
	}
	void	disp(Player & player)
	{
		system("cls");
		int	temp = map[player.getY()][player.getX()];
		map[player.getY()][player.getX()] = player.getSymbol();
		for (int i = 0;i<map.size();i++) {
			std::cout << map[i] << std::endl;
		}
		map[player.getY()][player.getX()] = temp;
	}
	bool	isMove(int x, int y)
	{
		if (map[y][x] != '|'
			&& map[y][x] != '-'
			&& map[y][x] != ' ') {
			return	true;
		}
		return	false;
	}
	void	action(Player & player)
	{
		// Item/Gold�Ȃǂ��E���A�����X�^�[�̏ꍇ�͐퓬�ɁB
		int	temp = map[player.getY()][player.getX()];
		switch (temp) {
		case '*':		// GOLD���E��
			player.addGold();
			map[player.getY()][player.getX()] = '.';
			player.setMessage("������E����");
			break;
		/*case ',':		// �C�F���_�[�̖��������E��
						// player���C�F���_�[�̖����������悤�ȃ����o�֐���ǉ�
			player.addAmulet();
			map[player.getY()][player.getX()] = '.';
			player.setMessage("�C�F���_�[�̖��������E����");
			break;*/
		case ':':	// food
			player.addFood();
			map[player.getY()][player.getX()] = '.';
			player.setMessage("�H�����E����");
			break;
		}
	}
	int	getMap(Player & player)
	{
		return map[player.getY()][player.getX()];
	}
	int	setMap(Player & player, int item)
	{
		map[player.getY()][player.getX()] = item;
	}
};


class Dungeon {
private:
	vector<Floor> floors;
	int		currentFloor;
public:
	Dungeon()
	{
		// �����K��ǂݍ���
		for (int i = 1;;i++) {
			Floor	floor;
			char	buf[128];
			sprintf(buf, "dungeon%02d.txt", i);
			if (!floor.load(buf)) {
				break;
			}
			floors.push_back(floor);
		}
		currentFloor = 0;
	}
	int		getFloorNumber()
	{
		return currentFloor + 1;
	}
	void	setPlayerPosi(Player & player)
	{
		floors[currentFloor].setPlayerPosi(player);
	}
	void	disp(Player & player)
	{
		floors[currentFloor].disp(player);
	}
	bool	isMove(int x, int y)
	{
		return	floors[currentFloor].isMove(x, y);
	}
	void	action(Player & player)
	{
		floors[currentFloor].action(player);
	}
	void	goUp(Player & player)
	{
		// �C�F���_�[�̖����������L���Ă��Ȃ��҂́A��鎖���ł��Ȃ��B
		/*if (!player.hasAmulet()) {
			return;
		}
		else {
			player.setMessage("�C�F���_�[�̖�������������܂���");
		}*/
		int item = floors[currentFloor].getMap(player);
		if (item == '%')
		{
			if (currentFloor > 0) {
				currentFloor--;
				floors[currentFloor].setPlayerPosi(player);
			}
			else {
				// Goal !!(�n��)
			}
		}
		else {
			player.setMessage("�K�i��������܂���");
		}
	}
	void	goDown(Player & player)
	{
		int item = floors[currentFloor].getMap(player);
		if (item == '%')
		{
			if (floors.size() - 1 > currentFloor) {
				currentFloor++;
				// Plyer�̍��W��ς���
				floors[currentFloor].setPlayerPosi(player);
			}
		}
		else {
			player.setMessage("�K�i��������܂���");
		}
	}
};

bool	inputCommand(int &add_x, int &add_y, int &cmd)
{
	string	command;
	add_x = 0;
	add_y = 0;
	cmd = 0;

	cin >> command;
	if (command == "Q") {
		return false;
	}
	else if (command == ".") {
		cmd = 4;				// rest
	}
	else if (command == "e") {
		cmd = 3;				// eat food
	}
	else if (command == "<") {
		cmd = 2;				// go up
	}
	else if (command == ">") {
		cmd = 1;				// go down
	}
	else if (command == "h") {
		add_x = -1;
	}
	else if (command == "j") {
		add_y = 1;
	}
	else if (command == "k") {
		add_y = -1;
	}
	else if (command == "l") {
		add_x = 1;
	}
	return true;
}

int main()
{
	Dungeon	dungeon;
	Player	player('@');
	int		add_x = 0;
	int		add_y = 0;
	int		cmd = 0;

	dungeon.setPlayerPosi(player);
	while (1) {
		dungeon.disp(player);
		player.disp(dungeon.getFloorNumber());
		if (!inputCommand(add_x, add_y, cmd)) {
			break;
		}
		if (cmd == 0) {
			if (dungeon.isMove(player.getX() + add_x, player.getY() + add_y)) {
				player.move(add_x, add_y);
				dungeon.action(player);
			}
		}
		else if (cmd == 1) {
			dungeon.goDown(player);
		}
		else if (cmd == 2) {
			dungeon.goUp(player);
		}
		else if (cmd == 3) {
			player.eatFood();
		}
		else if (cmd == 4) {
			player.rest();
		}
		if (player.isDead())
		{
			// GameOver
			system("cls");
			cout << "Game Over!!" << endl;
			break;
		}
	}
	return 0;
}