#include "stdafx.h"
#include "data.h"

void Data::createCharacter(string job)
{
	string createCharacter = job;

	if (createCharacter == "팔라딘")
	{
		//직업
		INIDATA->addData("CLASS", "0");

		//레벨
		INIDATA->addData("LEVEL", "1");

		//체력
		INIDATA->addData("MAX HP", "50");
		INIDATA->addData("HP", "50");

		//마나
		INIDATA->addData("MAX MP", "50");
		INIDATA->addData("MP", "50");

		//경험치
		INIDATA->addData("MAX EXP", "100");
		INIDATA->addData("EXP", "0");

		//위치
		INIDATA->addData("X coordinate", "600");
		INIDATA->addData("Y coordinate", "400");

		//스킬
		INIDATA->addData("SKILL POINT", "0");

		//치유력
		INIDATA->addData("HP REGEN", "0");
		INIDATA->addData("MP REGEN", "0.50");

		//데미지
		INIDATA->addData("DAMAGE", "10");
		INIDATA->addData("ATTACK DAMAGE", "0");
		INIDATA->addData("ABILITY POWER", "0");

		//방어력 및 저항력
		INIDATA->addData("ARMOR", "10");
		INIDATA->addData("MAGIC RESIST", "10");

		//크리티컬
		INIDATA->addData("CRITICAL", "0");

		//스피드
		INIDATA->addData("MOVE SPEED", "2");
		INIDATA->addData("DASH SPEED", "3");

		//회피력
		INIDATA->addData("EVASION", "0");

		//저항
		INIDATA->addData("RESISTANCE", "0");

		//행운
		INIDATA->addData("LUCK", "0");

		//소지 골드
		INIDATA->addData("GOLD", "10000");

		//소지 광물
		INIDATA->addData("ORE", "0");

		//물약
		INIDATA->addData("POTION MAX", "1");
		INIDATA->addData("POTION", "1");

		//소지 열쇠
		INIDATA->addData("bronzeKey", "0");
		INIDATA->addData("silverKey", "0");
		INIDATA->addData("goldKey", "0");
		INIDATA->addData("redKey", "0");

		//플레이타임
		INIDATA->addData("PLAY TIME", "0");
	}
	else if (createCharacter == "궁수")
	{
		//직업
		INIDATA->addData("CLASS", "1");

		//레벨
		INIDATA->addData("LEVEL", "1");

		//체력
		INIDATA->addData("MAX HP", "50");
		INIDATA->addData("HP", "50");

		//마나
		INIDATA->addData("MAX MP", "50");
		INIDATA->addData("MP", "50");

		//경험치
		INIDATA->addData("MAX EXP", "100");
		INIDATA->addData("EXP", "0");

		//위치
		INIDATA->addData("X coordinate", "600");
		INIDATA->addData("Y coordinate", "400");

		//스킬
		INIDATA->addData("SKILL POINT", "0");

		//치유력
		INIDATA->addData("HP REGEN", "0");
		INIDATA->addData("MP REGEN", "0.50");

		//데미지
		INIDATA->addData("DAMAGE", "10");
		INIDATA->addData("ATTACK DAMAGE", "0");
		INIDATA->addData("ABILITY POWER", "0");

		//방어력 및 저항력
		INIDATA->addData("ARMOR", "10");
		INIDATA->addData("MAGIC RESIST", "10");

		//크리티컬
		INIDATA->addData("CRITICAL", "0");

		//스피드
		INIDATA->addData("MOVE SPEED", "2");
		INIDATA->addData("DASH SPEED", "3");

		//회피력
		INIDATA->addData("EVASION", "0");

		//저항
		INIDATA->addData("RESISTANCE", "0");

		//행운
		INIDATA->addData("LUCK", "0");

		//소지 골드
		INIDATA->addData("GOLD", "10000");

		//소지 광물
		INIDATA->addData("ORE", "200");

		//물약
		INIDATA->addData("POTION MAX", "1");
		INIDATA->addData("POTION", "1");

		//소지 열쇠
		INIDATA->addData("bronzeKey", "0");
		INIDATA->addData("silverKey", "0");
		INIDATA->addData("goldKey", "0");
		INIDATA->addData("redKey", "0");

		//플레이타임
		INIDATA->addData("PLAY TIME", "0");
	}

	INIDATA->iniSave();
	INIDATA->listNum();
}
