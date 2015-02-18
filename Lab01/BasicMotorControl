FEHMotor motor(FEHMotor::Motor0);

int main(void)
{
    motor.SetPower(127);
    LCD.Write("Motor full power forward\n");
    Sleep(5000);
    motor.SetPower(0);
    LCD.Write("Motor off\n");
    Sleep(5000);
    motor.SetPower(-128);
    LCD.Write("Motor full power backward\n");
    Sleep(5000);
    motor.SetPower(0);
    LCD.Write("Motor off\n");


    return 0;
}
