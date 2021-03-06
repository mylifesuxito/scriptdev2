UPDATE `instance_template` SET `script`='instance_forge_of_souls' WHERE `map`=632;
UPDATE `creature_template` SET `ScriptName`='boss_bronjahm', `AIName` ='' WHERE `entry`=36497;
UPDATE `creature_template` SET `ScriptName`='mob_soul_fragment', `modelid_A`= 30233, `modelid_H`= 30233, `AIName` ='' WHERE `entry`=36535;
-- UPDATE `creature_template` SET `ScriptName`='mob_soul_storm', `AIName` ='' WHERE `entry`=;
-- UPDATE `creature_template` SET `ScriptName`='boss_devourer', `AIName` ='' WHERE `entry`=33113;

-- UPDATE `instance_template` SET `script`='instance_pit_of_saron' WHERE `map`=658;
-- UPDATE `creature_template` SET `ScriptName`='boss_ick', `AIName` ='' WHERE `entry`=36476;
-- UPDATE `creature_template` SET `ScriptName`='boss_krick', `AIName` ='' WHERE `entry`=36477;
-- UPDATE `creature_template` SET `ScriptName`='mob_exploding_orb', `AIName` ='' WHERE `entry`=36610;
-- UPDATE `creature_template` SET `ScriptName`='boss_forgemaster_garfrost', `AIName` ='' WHERE `entry`=36494;
-- UPDATE `creature_template` SET `ScriptName`='boss_scourgelord_tyrannus', `AIName` ='' WHERE `entry`=36658;

UPDATE `instance_template` SET `script`='instance_halls_of_reflection' WHERE `map`=668;
UPDATE `creature_template` SET `ScriptName`='boss_falryn' WHERE `entry`=38112;
UPDATE `creature_template` SET `ScriptName`='boss_marwyn' WHERE `entry`=38113;
-- UPDATE `creature_template` SET `ScriptName`='boss_lich_king_fh' WHERE `entry`=37226;
UPDATE `gameobject_template` SET ScriptName = 'go_frostmourne_altar' WHERE `entry` = 202236;
UPDATE `gameobject_template` SET ScriptName = 'go_frostmourne' WHERE `entry` = 202302;
DELETE FROM `creature` WHERE `map` = 668 AND `id` IN (38177,38176,38173,38172,38567,38175);
UPDATE `creature_template` SET `ScriptName`='generic_creature' WHERE `entry` IN (38177,38176,38173,38172,38567,38175);
UPDATE `gameobject_template` SET `faction` = '114' WHERE `entry` IN (197341, 201976);
