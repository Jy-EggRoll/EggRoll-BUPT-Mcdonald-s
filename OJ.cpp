#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define STRLEN 60
#define NUMFOOD 200
#define NUMCOMBO 200

int system_status = 1;//ϵͳ״̬��0.�رգ�1.����

//?��Ľṹ
typedef struct
{
    char name[STRLEN];//?�������
    int time;//��������Ҫ��ʱ��
    int max;//��?�洢����
    int capnow;//��ǰ����
    int time_not_finished;//����������?���Ѿ�?�˶�?ʱ��
} Food;

//�ײ͵Ľṹ
typedef struct
{
    char name[STRLEN];//�ײ͵�����
    int food_count;//?�������
    Food* food[6];//ָ��?��ṹ��
    int time_food_need[6];//��¼������Ҫ��ʱ�䣬�浽��Ӧ���
} Combo;

//�����Ľṹ
typedef struct
{
    char name[STRLEN];//����������
    int what;//����һ��ʳ�ﻹ��һ���ײͣ�1.ʳ�2.�ײ�
    Food* pfood;
    Combo* pcombo;
    long sec_time;//����������ʱ��
    int check_food[NUMFOOD];//�ж��ײ��ж�Ӧ��ŵ�?���Ƿ��Ѿ���ɣ�0.δ��ɣ�1.�����
    int status;//������״̬��0.�ڶ����У�1.�ɹ���2.ʧ��
    char* finish_time;//��ɵ�ʱ��
} Order;

int check_this_is_a_combo(const char* str, Combo* combos, int number_of_combo);
long convert_to_second(int hour, int minute, int second);
char* convert_to_time_format(long second_time);

int main(void)
{
    int open;
    int close;
    int number_of_food;
    int number_of_combo;
    Food foods[NUMFOOD];
    Combo combos[NUMCOMBO];
    FILE* file;
    char buffer[5000];
    file = fopen("dict.dic", "r");
    if (file == NULL)
    {
        printf("�޷����ļ�\n");
        return 1;
    }
    // ��ȡ��һ��
    fscanf(file, "%d %d\n", &number_of_food, &number_of_combo);
    // ��ȡ�ڶ���
    fgets(buffer, sizeof(buffer), file);
    // ʹ��strtok�ָ�ÿ��ʳ�ﲢ�洢���ṹ��������
    char* food_name = strtok(buffer, " ");
    int num_foods = 0;
    while (food_name != NULL)
    {
        strncpy(foods[num_foods].name, food_name, STRLEN - 1);
        foods[num_foods].name[STRLEN - 1] = '\0';
        num_foods++;
        food_name = strtok(NULL, " ");
    }
    int temp = strlen(foods[number_of_food - 1].name);
    if (foods[number_of_food - 1].name[temp - 1] == '\n')
    {
        foods[number_of_food - 1].name[temp - 1] = '\0';
    }
    for (int i = 0; i < number_of_food; i++)
    {
        fscanf(file, "%d", &foods[i].time);
    }
    for (int i = 0; i < number_of_food; i++)
    {
        fscanf(file, "%d", &foods[i].max);
    }
    fscanf(file, "%d %d\n", &close, &open);
    int food_index = 0;
    for (int i = 0; i < number_of_combo; i++)
    {
        int num_combos = 0;
        if (fgets(buffer, sizeof(buffer), file) == NULL)
        {
            break;//�����ļ���ȡ���������
        }
        char* name = strtok(buffer, " ");
        while (name != NULL)
        {
            if (num_combos == 0)
            {
                strncpy(combos[i].name, name, 60);
                combos[i].name[STRLEN - 1] = '\0';
                num_combos++;
                food_index = 0;
            }
            else
            {
                for (int j = 0; j < number_of_food; j++)
                {
                    if (!strcmp(name, foods[j].name))
                    {
                        combos[i].food[food_index] = &foods[j];
                        food_index++;
                        break;
                    }
                }
            }
            name = strtok(NULL, " ");
            if (name != NULL)
            {
                size_t temp = strlen(name);
                if (temp > 0 && name[temp - 1] == '\n')
                {
                    name[temp - 1] = '\0';
                }
            }
        }
        combos[i].food_count = food_index;
    }
    fclose(file);
    for (int i = 0; i < number_of_food; i++)
    {
        foods[i].capnow = 0;
        foods[i].time_not_finished = 0;
    }
    int number_of_order;
    int unfinish_order = 0;
    int hour;
    int minute;
    int second;
    scanf("%d", &number_of_order);
    Order* orders = (Order*)calloc(number_of_order, sizeof(Order));
    if (orders == NULL)
    {
        printf("ʧ��\n");
        exit(1);
    }
    int* count_unfinish_second = (int*)calloc(number_of_order, sizeof(int));
    if (count_unfinish_second == NULL)
    {
        printf("ʧ��\n");
        exit(1);
    }
    long save_time = 0;
    int count_var = 0;
    int* check = &count_var;
    int i = 0;
    scanf("%d:%d:%d", &hour, &minute, &second);
    orders[i].sec_time = convert_to_second(hour, minute, second);
    for (long time_from_zero = 0; time_from_zero <= 23 * 3600 + 59 * 60 + 59 - 7 * 3600; time_from_zero++)
    {
        if (time_from_zero == save_time + 1)
        {
            system_status = 1;
        }
        if (time_from_zero > 0)
        {
            for (int j = 0; j < number_of_food; j++)
            {
                if (foods[j].capnow < foods[j].max)
                {
                    foods[j].time_not_finished++;
                }
                if (foods[j].time_not_finished == foods[j].time)
                {
                    foods[j].capnow++;
                    foods[j].time_not_finished = 0;
                }
                if (foods[j].capnow == foods[j].max)
                {
                    foods[j].time_not_finished = 0;
                }
            }
        }
        if (time_from_zero == orders[i].sec_time)
        {
            scanf("%s", orders[i].name);
            if (check_this_is_a_combo(orders[i].name, combos, number_of_combo))
            {
                orders[i].what = 2;
            }
            else
            {
                orders[i].what = 1;
            }
            if (orders[i].what == 1)
            {
                for (int j = 0; j < number_of_food; j++)
                {
                    if (!strcmp(foods[j].name, orders[i].name))
                    {
                        orders[i].pfood = &foods[j];
                    }
                }
            }
            else
            {
                for (int j = 0; j < number_of_combo; j++)
                {
                    if (!strcmp(combos[j].name, orders[i].name))
                    {
                        orders[i].pcombo = &combos[j];
                        break;
                    }
                }
            }
            orders[i].status = 0;
            if (orders[i].sec_time > 22 * 3600 - 7 * 3600)
            {
                orders[i].status = 2;
                char fail[] = "Fail";
                orders[i].finish_time = fail;
            }
            if (i < number_of_order)
            {
                i++;
            }
            if (i < number_of_order)
            {
                scanf("%d:%d:%d", &hour, &minute, &second);
                orders[i].sec_time = convert_to_second(hour, minute, second);
            }
        }
        int j = *check;
        while (time_from_zero >= orders[j].sec_time && j < number_of_order)
        {
            if (orders[*check].status == 1 || orders[*check].status == 2)
            {
                (*check)++;
            }
            if (system_status == 0 && orders[j].sec_time == time_from_zero)
            {
                orders[j].status = 2;
                char fail[] = "Fail";
                orders[j].finish_time = fail;
            }
            if (orders[j].status == 0)
            {
                if (orders[j].what == 2)
                {
                    for (int m = 0; m < orders[j].pcombo->food_count; m++)
                    {
                        if (orders[j].pcombo->food[m]->capnow > 0 && orders[j].check_food[m] == 0)
                        {
                            orders[j].pcombo->food[m]->capnow--;
                            orders[j].check_food[m] = 1;
                        }
                    }
                    if (orders[j].status != 2)
                    {
                        orders[j].status = 1;
                    }
                    for (int m = 0; m < orders[j].pcombo->food_count; m++)
                    {
                        if (orders[j].check_food[m] == 0)
                        {
                            orders[j].status = 0;
                        }
                    }
                    if (orders[j].status == 1)
                    {
                        if (time_from_zero != orders[j].sec_time)
                        {
                            unfinish_order--;
                        }
                        if (unfinish_order < open)
                        {
                            save_time = time_from_zero;
                        }
                        orders[j].finish_time = convert_to_time_format(time_from_zero);
                    }
                    else
                    {
                        count_unfinish_second[j]++;
                        if (count_unfinish_second[j] == 1)
                        {
                            unfinish_order++;
                            if (unfinish_order > close)
                            {
                                system_status = 0;
                            }
                        }
                    }
                }
                else
                {
                    if (orders[j].pfood->capnow > 0)
                    {
                        orders[j].pfood->capnow--;
                        orders[j].status = 1;
                        if (time_from_zero != orders[j].sec_time)
                        {
                            unfinish_order--;
                        }
                        if (unfinish_order < open)
                        {
                            save_time = time_from_zero;
                        }
                        orders[j].finish_time = convert_to_time_format(time_from_zero);
                        if (j == *check)
                        {
                            (*check)++;
                        }
                    }
                    else
                    {
                        count_unfinish_second[j]++;
                        if (count_unfinish_second[j] == 1)
                        {
                            unfinish_order++;
                            if (unfinish_order > close)
                            {
                                system_status = 0;
                            }
                        }
                    }
                }
            }
            j++;
        }
    }

    for (int i = 0; i < number_of_order; i++)
    {
        printf("%s\n", orders[i].finish_time);
    }

    free(orders);
    free(count_unfinish_second);

    return 0;
}

int check_this_is_a_combo(const char* str, Combo* combos, int number_of_combo)
{
    for (int i = 0; i < number_of_combo; i++)
    {
        if (!strcmp(str, combos[i].name))
        {
            return 1;
        }
    }
    return 0;
}

long convert_to_second(int hour, int minute, int second)
{
    return hour * 3600 + minute * 60 + second - 7 * 3600;
}

char* convert_to_time_format(long second_time)
{
    int hour = (second_time) / 3600 + 7;
    int minute = (second_time % 3600) / 60;
    int second = second_time % 60;
    char* time_str = (char*)calloc(10, sizeof(char));
    sprintf(time_str, "%02d:%02d:%02d", hour, minute, second);
    return time_str;
}