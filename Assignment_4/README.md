Author : Giorgos Mathioudakis - csd4674

Note that the following code does not compile because of bugs that I could not fix in days of trying but the general idea should work .

The guide in steps : 

1. Implement a mechanism to find tasks that use the 'Group Fairness' Policy by checking if a task has a group name 'A-Z' and a member id > 0 . These tasks can be stored in a data structure so we dont go through the whole red-black tree every time . We can achieve this by initializing those values inside the copy_process function at fork.c. 

2. In a while loop we traverse the rb tree and and find out how many groups there are . Also we find out how many processes there are in each of the groups.

3. Using the num of processes in each group and num of groups we update the time slice of each process using the formula:
Τ(process_params, number_of_groups) = 100/number_of_groups/number_of_processes_in_group(process_params.group_name) .

4. We now select the process with the highest calculated priority and make that target .

5. We monitor the progress of the processes always and make sure to adjust when we should . At any moment a new process could start running in a group and that changes the time slice of every process in that group . Also if any process is running for longer than its time slice we need to ensure its stopped and priority should be given to another one .


The implementation that I would do written in C.

In sched.c at pick_next_task we add the following code. If the following code does not return any tasks then we continue with the default code of pick_next_task.

void update_time_slices_for_group(struct group_info *group, float time_slice) {
    // Traverse each process in the group and update its time slice
    struct task_struct *p;
    for each process p in group {
        set_time_slice(p, time_slice); // Set the time slice for this process
    }
}

float calculate_priority(struct task_struct *task) {
    // Example calculation; actual implementation may vary
    // Higher value means higher priority
    float group_cpu_share = get_group_cpu_share(task->group_name); // Get the CPU share for this group
    float task_priority = static_cast<float>(task->prio);
    float time_slice_used_ratio = (task->time_slice_used / task->time_slice_total);

    // Example formula - this can be adjusted based on requirements
    return (group_cpu_share * task_priority) / (1 + time_slice_used_ratio);
}


// This code snippet is inside the function pick_next_task() inside the file sched.c
...
// Firstly we get the first node of the rb tree.
struct rb_node* current = rb_first(&rq->cfs.tasks_timeline);
// Then we get the task with highest priority.
struct sched_entity* run_node;
struct task_struct* task;

run_node = container_of(current, struct sched_entity, run_node);
task = task_of(run_node);

float highest_priority_value = -1;
struct task_struct* target_s = NULL;

while( current != NULL) {
	run_node = container_of(current, struct sched_entity, run_node);
	task = task_of(run_node);

	if ( task->group_name != '' && task->member_id > 0 ) {
    int num_processes = count_processes_in_group(task->group_name); // Loop through all rb nodes and find how many processes are in this group
    int total_groups = count_groups(); // Loop through all rb nodes and find how many groups there are
    float time_slice_per_process = 100.0 / total_groups / num_processes;

    // Update the time slice for each process in this group
    update_time_slices_for_group(group, time_slice_per_process);

    float priority_value = calculate_priority(task);
    // Select the task with the highest calculated priority
    if (priority_value > highest_priority_value) {
        highest_priority_value = priority_value;
        target_s = task;
    }

  }

  current = rb_next(current);
}

if ( target_s != NULL ) {
		return target_s;
}
...
