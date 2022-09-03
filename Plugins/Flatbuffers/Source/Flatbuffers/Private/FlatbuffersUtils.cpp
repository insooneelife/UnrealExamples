#include "FlatbuffersUtils.h"
#include "Serialization/MemoryWriter.h"
#include "monster_generated.h"


void FlatbuffersUtils::Serialize(TArray<uint8>& OutPayload)
{
	flatbuffers::FlatBufferBuilder Fbb;

	MyGame::Sample::Vec3 Pos(1, 2, 3);

	auto WeaponName1 = Fbb.CreateString("weaponName1");
	auto WeaponName2 = Fbb.CreateString("weaponName2");

	auto Weapon1 = MyGame::Sample::CreateWeapon(Fbb, WeaponName1, 18);
	auto Weapon2 = MyGame::Sample::CreateWeapon(Fbb, WeaponName2, 28);

	std::vector<flatbuffers::Offset<MyGame::Sample::Weapon>> Weapons;

	Weapons.push_back(Weapon1);
	Weapons.push_back(Weapon2);

	auto WeaponsVec = Fbb.CreateVector(Weapons);
	auto MonsterName = Fbb.CreateString("monsterName");

	int32 mana = 16;
	int32 hp = 15;
	int8 inventory = 5;
	MyGame::Sample::Color Color = MyGame::Sample::Color::Color_Blue;
	auto Monster = MyGame::Sample::CreateMonster(Fbb, &Pos, mana, hp, MonsterName, inventory, Color, WeaponsVec, MyGame::Sample::Equipment::Equipment_Weapon);

	Fbb.Finish(Monster);
	FMemoryWriter WriterArray(OutPayload);
	WriterArray.Serialize(Fbb.GetBufferPointer(), Fbb.GetSize());
}

void FlatbuffersUtils::Deserialize(const TArray<uint8>& InPayload, FMonsterData& OutData)
{
	const MyGame::Sample::Monster& Monster = *flatbuffers::GetRoot<MyGame::Sample::Monster>(InPayload.GetData());

	auto Name = FString(Monster.name()->c_str()) ;
	auto Color = Monster.color();

	UE_LOG(LogTemp, Log, TEXT("Name : %s Color : %d"), *Name, Color);
}