Overview
--------
- This game is a basic tower defense game.
- There are 3 towers viz. Physical (Red), Magical (Blue) & Slowing (Black)
- There are 4 enemies. The larger the enemies are, the slower and tankier they are. Some enemies have armor while some others have magic resistance.
- Enemies spawn in waves and the player must place towers on the green circles to defend the castle which is in the center of the map.
- The game is won when the last enemy of the last wave has been killed. The game is lost if the player lets 10 enemies reach the castle.


Assignment 3
------------
- Once you hit Play in the editor, double-click the green circles on the map to bring up a tower selection. Then select a tower of your choice to construct one.
- The enemy waves start automatically and there will be a UI text element that will notify you when each wave starts, when you win and when you lose.

Model
-----
- I decided to stick with Unreal's naming conventions and folder structures. And surprisingly, this resulted in a very organic segregation of classes/blueprints into the MVC pattern:
			
1. Enemies
----------
Model  		~/Source/Enemies/BasicEnemy
				~/Content/Enemies/BP_BasicEnemy
			~/Source/Shared/WaveData
				~/Content/Enemies/LongEnemyWaveData
				~/Content/Enemies/ShortEnemyWaveData
			~/Source/Shared/PathDataActor

View 		~/Content/Enemies/BP_HugeEnemy
			~/Content/Enemies/BP_LargeEnemy
			~/Content/Enemies/BP_MediumEnemy
			~/Content/Enemies/BP_SmallEnemy

Controller 	~/Source/Enemies/EnemySpawner
				~/Content/Enemies/BP_EnemySpawner
			~/Source/Shared/SpawnerComponent

2. Towers
---------
Model		~/Source/Towers/BasicTower
				~/Source/Towers/BasicProjectile

View		~/Content/Towers/BP_PhysicalTower
				~/Content/Towers/BP_PhysicalProjectile
			~/Content/Towers/BP_MagicalTower
				~/Content/Towers/BP_MagicalProjectile
			~/Content/Towers/BP_SlowingTower
				~/Content/Towers/BP_TowerSpawner

Controller 	~/Source/Towers/TowerSpawner

3. Damage
---------
Model 		~/Source/StatsBuffs/Buff
			~/Source/StatsBuffs/StatEffect
				~/Content/Effects/PhysicalDamageEffect
				~/Content/Effects/MagicDamageEffect
				~/Content/Effects/SlowingEffect

4. Player & General
-------------------
Model 		~/Source/Shared/EAE_6900GameModeBase
				~/Content/Shared/BP_GameMode

View 		~/Content/UI/W_EnemyInfo
			~/Content/UI/W_EventLogItem
			~/Content/UI/W_GameUI
			~/Content/UI/W_TowerSelect

Controller	~/Source/Shared/TDPlayerController
				~/Content/BP_TDPlayerController
