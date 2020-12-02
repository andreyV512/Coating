#pragma once

template<class Owner, class Tresh> void SetTresholds(Owner &o, Tresh &t)
{
	o.threshAlarm = t.get<AlarmThresh>().value;
	o.offsAlarmStart = int(t.get<AlarmThreshStart>().value * o.packetSize * 0.01);
	o.offsAlarmStop = int(t.get<AlarmThreshStop>().value * o.packetSize * 0.01);
	o.gainAlarmOffs = t.get<AlarmGainStart>().value;
	o.gainAlarmDelta = (t.get<AlarmGainStop>().value - t.get<AlarmGainStart>().value)
		/ (o.offsAlarmStop - o.offsAlarmStart);

	o.threshReflection = t.get<BottomReflectionThresh>().value;
	o.offsReflectionStart = int(t.get<BottomReflectionThreshStart>().value * o.packetSize * 0.01);
	o.offsReflectionStop = int(t.get<BottomReflectionThreshStop>().value * o.packetSize * 0.01);
	o.gainReflectionOffs = t.get<BottomReflectionGainStart>().value;
	o.gainReflectionDelta = (t.get<BottomReflectionGainStop>().value - t.get<BottomReflectionGainStart>().value)
		/ (o.offsReflectionStop - o.offsReflectionStart);
	o.bottomReflectionOn = t.get<BottomReflectionOn>().value;
}