#ifndef PATIENT_H
#define PATIENT_H

typedef struct patient Patient;
typedef struct list_patient LinkedPatientList;
typedef struct list_patient_node ListPatientNode;

// Initialize the dynamic list of patients
LinkedPatientList *ll_patient_create();

// Check if the Patient list is empty
int ll_patient_is_empty(LinkedPatientList *l);

//Create a new Patient
Patient *generate_patient();

// Insert patient into the list
void ll_patient_insert(LinkedPatientList *l, Patient *p);

// Counter to see the number of elements in the patient list
int ll_patient_size(LinkedPatientList *l);

//Free allocated memory
void ll_patient_free(LinkedPatientList *l);

//Get the ID of Patient and return for main
int get_patient_id(Patient *p);

#endif