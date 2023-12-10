#include "patient.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// Define the struct for the patient create
struct patient 
{
    int id, age;
    char name[50], cpf[12];
};

// Define the struct for the list of patients
struct list_patient
{
   ListPatientNode *first; 
};

// Define the struct for the patients nodes
struct list_patient_node
{
   Patient *info;       
   ListPatientNode *next; 
};

// Initialize the dynamic list of patients
LinkedPatientList *ll_patient_create()
{
   LinkedPatientList *l = (LinkedPatientList *)malloc(sizeof(LinkedPatientList)); 
   l->first = NULL;                                          
   return l;                                                
}

// Check if the Patient list is empty
int ll_patient_is_empty(LinkedPatientList *l)
{
   return l->first == NULL; 
}

// Generate a name for Patient
static void gen_patient_name(Patient *p, int length) {
   char name[length+1];
   name[0] = gen_randint('A', 'Z');  // Using ASCII table as integer
   for (int i=1; i < length; i++)
      name[i] = gen_randint('a', 'z');
   name[length] = '\0';
   strcpy(p->name, name);
}

// Generate a CPF for Patient
static void gen_patient_cpf(Patient *p) {
   char cpf[12];
   for (int i=0; i < 11; i++)
      cpf[i] = gen_randint('0', '9');
   cpf[11] = '\0';
   strcpy(p->cpf, cpf);
}

// Create a new Patient
Patient *generate_patient() {
   static int id;
   Patient *p = malloc(sizeof(Patient));
   p->id = id++;
   p->age = gen_randint(20, 80);
   gen_patient_name(p, gen_randint(4, 8));
   gen_patient_cpf(p);
   return p;
}

// Insert patient into the list
void ll_patient_insert(LinkedPatientList *l, Patient *p)
{
   ListPatientNode *node = (ListPatientNode *)malloc(sizeof(ListPatientNode));
   node->info = p;                                      
   node->next = l->first;                               
   l->first = node;                                     
}

// Counter to see the number of elements in the patient list
int ll_patient_size(LinkedPatientList *l)
{
   int cnt = 0;
   for (ListPatientNode *p = l->first; p != NULL; p = p->next)
   {
      cnt++; 
   }
   return cnt; 
}

//Free allocated memory
void ll_patient_free(LinkedPatientList *l)
{
   ListPatientNode *p = l->first;
   while (p != NULL)
   {
      ListPatientNode *t = p->next; 
      free(p);               
      p = t;                 
   }
   free(l);
}

//Get the ID of Patient and return for main 
int get_patient_id(Patient *p) {
   return p->id;
}