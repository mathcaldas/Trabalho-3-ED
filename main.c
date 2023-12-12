#include "utils.h"
#include "awaitable.h"
#include "exam.h"
#include "patient.h"
#include "report.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Constants for configuration
#define Q_REPORT_TIME_LIMIT 7200
#define SLEEP_TIME 5
#define DEVICE_SIZE 5
#define DEVICE_LOWER_LIMIT 5
#define DEVICE_UPPER_LIMIT 10
#define RADIOLOGIST_SIZE 3
#define RADIOLOGIST_LOWER_LIMIT 10
#define RADIOLOGIST_UPPER_LIMIT 30


int main() {
    // Uncomment to generate different simulation cases
    // srand(time(NULL));

    // Initialize queues, lists, and awaitables
    ExamQueue *q_exam = exq_create();
    ReportQueue *q_report = req_create();
    LinkedPatientList *lpl = ll_patient_create();

    Awaitable **device = create_awaitables(DEVICE_SIZE);
    Awaitable **radiologist = create_awaitables(RADIOLOGIST_SIZE);

    // Initialize log variable
    Log *log = create_log();
    
    // Declare msg for log
    char msg[150];

    // Variables for metrics
    int acc_cont_patient_exams, acc_pathology_time;
    float exam_time_result = 0;
    int pathology_exams_time[5] = {0};
    int cont_pathology_exams[5] = {0};
    float avg_pathology_time[5] = {0};

    // Simulation loop (43200 iterations)
    for (int i = 0; i < 43200; i++) {
        // Randomly generate patients and enqueue them into the exam queue
        if (gen_randint(0, 100) <= 20){
            Patient *p = generate_patient();
            ll_patient_insert(lpl, p);
            
            char patient_name[50], patient_cpf[12];
            int age, id = get_patient_id(p);
            ll_get_patient_attributes(lpl, id, patient_name, patient_cpf, &age);
            int patient_id = get_patient_id(p);
            sprintf(msg, "ID: %5d | Patient Created (CPF: %s; Name: %s; Age: %d)", patient_id, patient_cpf, patient_name, age);
            log_event(log, msg);
            exq_enqueue(q_exam, patient_id);
        }

        // Process exams using available devices
        for (int j = 0; j < DEVICE_SIZE; j++) {
            if (!exq_is_empty(q_exam) && is_awaitable_available(device[j])) {
                // Dequeue a patient from the exam queue and use the device
                int patient_id = exq_dequeue(q_exam);
                int duration = use_awaitable(device[j], patient_id, DEVICE_LOWER_LIMIT, DEVICE_UPPER_LIMIT);
                sprintf(msg, "ID: %5d | Patient is using device (Device duration: %d)", patient_id, duration);
                log_event(log, msg);
            }
            else if (!is_awaitable_available(device[j]) && awaitable_duration_decrease(device[j]) == 0) {
                // If the device is no longer busy, enqueue the patient for report
                int patient_id = get_awaitable_id(device[j]);
                sprintf(msg, "ID: %5d | Patient went to report queue", patient_id);
                log_event(log, msg);
                req_enqueue(q_report, patient_id, i);
            }
        }
        // Process exams using available radiologists
        for (int j = 0; j < RADIOLOGIST_SIZE; j++){
            req_clear(q_report, i, Q_REPORT_TIME_LIMIT, pathology_exams_time, cont_pathology_exams);
            if (!req_is_empty(q_report) && is_awaitable_available(radiologist[j])){
                // Dequeue an exam from the report queue and use the radiologist
                Condition condition;
                int patient_id, initialization;
                Exam *exam = req_dequeue(q_report);
                req_get_exam_attributes(exam, &patient_id, &initialization, &condition);
                free(exam);
                int queue_time = get_qtime_to_exam(i, initialization);

                pathology_exams_time[condition] += queue_time;
                cont_pathology_exams[condition]++;
                int duration = use_awaitable(radiologist[j], patient_id, RADIOLOGIST_LOWER_LIMIT, RADIOLOGIST_UPPER_LIMIT);

                sprintf(msg, "ID: %5d | Patient went to radiologist (Elapsed report queue time: %d; Patient Condition: %s; Radiologist duration: %d)", patient_id, queue_time, get_condition_name(condition), duration);
                log_event(log, msg);
            }
            else if (!is_awaitable_available(radiologist[j]) && awaitable_duration_decrease(radiologist[j]) == 0) {
                int patient_id = get_awaitable_id(radiologist[j]);
                sprintf(msg, "ID: %5d | Patient finished reports", patient_id);
                log_event(log, msg);
            }
        }
        
        // Display statistics every 10 iterations
        if (i % 10 == 0) {
            acc_cont_patient_exams = acc_pathology_time = 0;
            for (int i=HEALTHY; i <= APPENDICITIS; i++) {
                acc_pathology_time += pathology_exams_time[i];
                acc_cont_patient_exams += cont_pathology_exams[i];
                avg_pathology_time[i] = cont_pathology_exams[i] > 0 ? (float)pathology_exams_time[i]/cont_pathology_exams[i] : 0;
            }
            exam_time_result = acc_cont_patient_exams > 0 ? (float)acc_pathology_time/acc_cont_patient_exams : 0;
            printf("Iter %d: Tempo medio: %.2f\nPor condicao: |", i, exam_time_result);
            for (int i=HEALTHY; i <= APPENDICITIS; i++)
                printf(" %s: %.2f |", get_condition_name(i), avg_pathology_time[i]);
            printf("\n");
            ms_sleep(SLEEP_TIME);
        }
    }

    // Process remaining data after the simulation ends
    printf("\nGerenciando filas com dados remanescentes: \n");
    const int passed_limit_time = req_length(q_report);

    for (int i = 43200; !exq_is_empty(q_exam) || !req_is_empty(q_report) || is_any_awaitable_busy(device, DEVICE_SIZE) || is_any_awaitable_busy(radiologist, RADIOLOGIST_SIZE); i++) {
        // Randomly generate patients and enqueue them into the exam queue
        for (int j = 0; j < DEVICE_SIZE; j++) {
            if (!exq_is_empty(q_exam) && is_awaitable_available(device[j])) {
                // Dequeue a patient from the exam queue and use the device
                int patient_id = exq_dequeue(q_exam);
                log_event(log, msg);
                int duration = use_awaitable(device[j], patient_id, DEVICE_LOWER_LIMIT, DEVICE_UPPER_LIMIT);

                sprintf(msg, "ID: %5d | Patient is using device after time limit (Device duration: %d)", patient_id, duration);
            }
            else if (!is_awaitable_available(device[j]) && awaitable_duration_decrease(device[j]) == 0) {
                // If the device is no longer busy, enqueue the patient for report
                int patient_id = get_awaitable_id(device[j]);
                sprintf(msg, "ID: %5d | Patient went to report queue after time limit", patient_id);
                log_event(log, msg);
                req_enqueue(q_report, patient_id, i);
            }
        }
        // Process exams using available radiologists
        for (int j = 0; j < RADIOLOGIST_SIZE; j++){
            req_clear(q_report, i, Q_REPORT_TIME_LIMIT, pathology_exams_time, cont_pathology_exams);
            if (!req_is_empty(q_report) && is_awaitable_available(radiologist[j])){
                // Dequeue an exam from the report queue and use the radiologist
                Condition condition;
                int patient_id, initialization;
                Exam *exam = req_dequeue(q_report);
                req_get_exam_attributes(exam, &patient_id, &initialization, &condition);
                free(exam);
                int queue_time = get_qtime_to_exam(i, initialization);

                pathology_exams_time[condition] += queue_time;
                cont_pathology_exams[condition]++;
                int duration = use_awaitable(radiologist[j], patient_id, RADIOLOGIST_LOWER_LIMIT, RADIOLOGIST_UPPER_LIMIT);

                sprintf(msg, "ID: %5d | Patient went to radiologist after time limit (Elapsed report queue time: %d; Patient Condition: %s; Radiologist duration: %d)", patient_id, queue_time, get_condition_name(condition), duration);
                log_event(log, msg);
                                                                    
            }
            else if (!is_awaitable_available(radiologist[j]) && awaitable_duration_decrease(radiologist[j]) == 0) {
                int patient_id = get_awaitable_id(radiologist[j]);
                sprintf(msg, "ID: %5d | Patient finished reports after time limit", patient_id);
                log_event(log, msg);
            }
        }

        // Display statistics every 10 iterations
        if (i % 10 == 0) {
            acc_cont_patient_exams = acc_pathology_time = 0;
            for (int i=HEALTHY; i <= APPENDICITIS; i++) {
                acc_pathology_time += pathology_exams_time[i];
                acc_cont_patient_exams += cont_pathology_exams[i];
                avg_pathology_time[i] = cont_pathology_exams[i] > 0 ? (float)pathology_exams_time[i]/cont_pathology_exams[i] : 0;
            }
            exam_time_result = acc_cont_patient_exams > 0 ? (float)acc_pathology_time/acc_cont_patient_exams : 0;
            printf("Iter %d: Tempo medio: %.2f\nPor condicao: |", i, exam_time_result);
            for (int i=HEALTHY; i <= APPENDICITIS; i++)
                printf(" %s: %.2f |", get_condition_name(i), avg_pathology_time[i]);
            printf("\n");
            ms_sleep(SLEEP_TIME);
        }
    }

    // Final metrics and cleanup
    acc_pathology_time = acc_cont_patient_exams = 0;
    
    for (int i=HEALTHY; i <= APPENDICITIS; i++) {
        acc_pathology_time += pathology_exams_time[i];
        acc_cont_patient_exams += cont_pathology_exams[i];
        avg_pathology_time[i] = cont_pathology_exams[i] > 0 ? (float)pathology_exams_time[i]/cont_pathology_exams[i] : 0;
    }
    
    // Calculate the average exam time result
    exam_time_result = acc_cont_patient_exams > 0 ? (float)acc_pathology_time/acc_cont_patient_exams : 0;

    printf("\nResultado do tempo medio na fila para laudo: %.2f\n\n", exam_time_result);
    
    printf("Resultado do tempo médio por condicao:\n");
    for (int i=HEALTHY; i <= APPENDICITIS; i++)
        printf("%s: %.2f\n", get_condition_name(i), avg_pathology_time[i]);
    printf("\n");
    printf("Pacientes que ultrapassaram o tempo limite: %d\n", passed_limit_time);
    printf("\nQUANTIDADE DE PACIENTES NA LISTA: %d\n", ll_patient_size(lpl));

    // Create log file
    save_log_to_file(log, "log.txt", exam_time_result, avg_pathology_time, passed_limit_time, ll_patient_size(lpl));

    // Free allocated memory of queues, lists, and awaitables
    awaitable_free(device, DEVICE_SIZE);
    awaitable_free(radiologist, RADIOLOGIST_SIZE);
    ll_patient_free(lpl);
    exq_free(q_exam);
    req_free(q_report);
    log_free(log);
    
    return 0;
}