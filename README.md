# BLOSOM Algorithm

The BLOSOM framework allows one to mine arbitrary frequent  boolean
expressions include AND clauses (itemsets), OR clauses, and CNF/DNF
expressions. It focuses on mining the minimal boolean expressions.

'''mug''' finds minimal or-clauses, and-clauses, CNF and DNF expressions

'''xng''' finds closed  CNF expressions

'''xug''' finds closed DNF expressions.

cd to the appropriate directory and run make

the ibm directory has some IBM format synthetic datasets generated via
http://github.com/zakimjz/IBMGenerator

See also CHARM-L (http://github.com/zakimjz/CHARM-L) that can mine all minimal and closed and-clauses, i.e., all minimal and closed frequent itemsets.

*'''Relevant Publications'''
** [2006-blosom] Lizhuang Zhao, Mohammed J. Zaki, and Naren Ramakrishnan. Blosom: a framework for mining arbitrary boolean expressions. In 12th ACM SIGKDD International Conference on Knowledge Discovery and Data Mining. August 2006.

** [2010-ijkdb] Mohammed J. Zaki, Naren Ramakrishnan, and Lizhuang Zhao. Mining frequent boolean expressions: application to gene expression and regulatory modeling. International Journal of Knowledge Discovery in Bioinformatics, 1(3):68–96, September 2010. special issue on Mining Complex Structures in Biology.
