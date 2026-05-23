// Smart Glasses for Blind People
// Ultrasonic Sensor + Buzzer + Arduino Uno

#define trigPin 9
#define echoPin 10
#define buzzer 8

long duration;
int distance;

void setup()
{
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(buzzer, OUTPUT);

    Serial.begin(9600);
}

void loop()
{
    // Send ultrasonic pulse
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);

    digitalWrite(trigPin, LOW);

    // Read echo time
    duration = pulseIn(echoPin, HIGH);

    // Calculate distance
    distance = duration * 0.034 / 2;

    Serial.print("Distance: ");
    Serial.println(distance);

    // Obstacle detection
    if(distance <= 50)
    {
        digitalWrite(buzzer, HIGH);   // buzzer ON
    }
    else
    {
        digitalWrite(buzzer, LOW);    // buzzer OFF
    }

    delay(200);
}