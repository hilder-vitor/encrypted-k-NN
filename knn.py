from sklearn.neighbors import KNeighborsClassifier
from sklearn.metrics import confusion_matrix
import numpy as np
#from numpy import genfromtxt
#from numpy import array

from optparse import OptionParser

from timeit import default_timer as timer

#training_data = 'datasets/wine/wine.data.ope.training'
#testing_data = 'datasets/wine/wine.data.ope.testing'

#training_data = 'datasets/wine/wine.data.training'
#testing_data = 'datasets/wine/wine.data.testing'

#training_data = 'datasets/abalone/abalone.data.ope.training'
#testing_data = 'datasets/abalone/abalone.data.ope.testing'

#training_data = 'datasets/abalone/abalone.data.training'
#testing_data = 'datasets/abalone/abalone.data.testing'

#training_data = 'datasets/iris/iris.data.training'
#testing_data = 'datasets/iris/iris.data.testing'

#training_data = 'datasets/iris/iris.data.ope.training'
#testing_data = 'datasets/iris/iris.data.ope.testing'

#training_data = 'datasets/climate/climate.data.training'
#testing_data = 'datasets/climate/climate.data.testing'

#training_data = 'datasets/climate/climate.data.ope.training'
#testing_data = 'datasets/climate/climate.data.ope.testing'

#training_data = 'datasets/credit/credit.data.training'
#testing_data = 'datasets/credit/credit.data.testing'

#training_data = 'datasets/credit/credit.data.ope.training'
#testing_data = 'datasets/credit/credit.data.ope.testing'


parser = OptionParser()
parser.add_option("-f", dest="base_filename", help="base name of the training set and testing set files")
parser.add_option("-k", type="int", dest="k", help="number of nearest neighbors")
parser.add_option("-m", dest="method", default='uniform', help="method: uniform or distance")

(options, args) = parser.parse_args()

#knn = KNeighborsClassifier(n_neighbors=int(options.k), weights='distance', algorithm='brute')
knn = KNeighborsClassifier(n_neighbors=int(options.k), weights=options.method, algorithm='brute')

# reading the training data
training_data = options.base_filename + ".training"
data_and_classes = np.genfromtxt(training_data, dtype='int', skip_header=1)
#data = np.array(data_and_classes[:, 0:(len(data_and_classes[0]) - 1)])
data = data_and_classes[:, 0:(len(data_and_classes[0]) - 1)]
#classes = np.array(data_and_classes[:, (len(data_and_classes[0]) - 1)])
classes = data_and_classes[:, (len(data_and_classes[0]) - 1)]
knn.fit(data, classes)

# reading the testing data
testing_data = options.base_filename + ".testing"
data_and_classes = np.genfromtxt(testing_data, dtype='int', skip_header=1)
data = data_and_classes[:, 0:(len(data_and_classes[0]) - 1)]
#data = np.array(data_and_classes[:, 0:(len(data_and_classes[0]) - 1)])
classes = data_and_classes[:, (len(data_and_classes[0]) - 1)]
#classes = np.array(data_and_classes[:, (len(data_and_classes[0]) - 1)])

start = timer()
print(knn.score(data, classes))
end = timer()
print("Time to classify %d instances: %lf  (average time: %lf)" % (len(data), end - start, (end-start)/len(data)))

prediction = knn.predict(data);

outputfile = open(options.base_filename + ".prediction" + ".k" + str(options.k)  , "w+");
for class_predicted in prediction:
    outputfile.write(str(class_predicted) + "\n")

outputfile.close()
#print prediction
#print confusion_matrix(classes, prediction);
