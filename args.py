### params
d = 3
n = 1000
k = 100
pf = 'linear'
I = 'hv'
alg = 'fils-rlist-nlist'
rlist_size = 20
nlist_size = 20
id = 0 # random seed


import os
point_set_file_path = f'./ref_point_dataset/{pf}_d{d}_n{n}.csv'
res_dir_path = os.path.join('./result', f'pf-{pf}_d{d}_n{n}_k{k}_I-{I}_alg-{alg}_rsz{rlist_size}_nsz{nlist_size}')
os.makedirs(res_dir_path, exist_ok = True)

out_point_subset_file_path = os.path.join(res_dir_path, f'subset_{id}th_run.csv')
out_qi_file_path = os.path.join(res_dir_path, f'qi_{id}th_run.csv')
out_time_file_path = os.path.join(res_dir_path, f'time_{id}th_run.csv')
out_n_qi_calls_file_path = os.path.join(res_dir_path, f'n_qi_calls_{id}th_run.csv')

args = ['./selector']
args.append(f'-point_set_file_path {point_set_file_path}')
args.append(f'-point_subset_size {k}')
args.append(f'-q_indicator {I}')
args.append(f'-selector_alg {alg}')
args.append(f'-seed {id}')
args.append(f'-out_point_subset_file_path {out_point_subset_file_path}')
args.append(f'-out_qi_file_path {out_qi_file_path}')
args.append(f'-out_time_file_path {out_time_file_path}')
args.append(f'-out_n_qi_calls_file_path {out_n_qi_calls_file_path}')
args.append(f'-nlist_size {nlist_size}')
args.append(f'-rlist_size {rlist_size}')

if I in ['hv', 'nr2']:
    args.append(f'-hv_ref_point_val {1.1}')
if I in ['igd', 'igd+', 'epsilon']:
    args.append(f'-ref_point_set_file_path {point_set_file_path}')
if I in ['r2', 'nr2']:
    args.append(f'-weight_point_set_file_path ./weight_point_dataset/d{d}_n{n}.csv')

print(' '.join(args))