#include <iostream>
#include <math.h>
#include <unistd.h>

class RobotModel{
    public:
        RobotModel(double new_x_,  double new_y_, double new_theta_){
            x_ = new_x_; y_ = new_y_; theta_ = new_theta_;
        }

        void Update(double v, double omega){
            x_ += dt_*v*cos(theta_);
            y_ += dt_*v*sin(theta_);
            theta_ += dt_*omega;
        }

        void GetState(double *p_x, double *p_y, double *p_theta){
            *p_x = x_;
            *p_y = y_;
            *p_theta = theta_;
        }

    private:
        double x_, y_, theta_;
        double dt_ = 0.02;
};

int main(){
    RobotModel rm_1(10.0, 10.0, 3.1416/3.0);
    double x, y, theta;

    while(1){
        usleep(1000000);
        rm_1.Update(0.1, 0);
        rm_1.GetState(&x, &y, &theta);
        std::cout<<"Robot state: x["<<x<<"] y["<<y<<"] theta["<<theta<<"]"<<std::endl;

    }

    return 0;
}
