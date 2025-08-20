# AVIDNet: Attribute-aware Vehicle Inference Detector Network
A vehicle-detection deep learning network based on the ResNet50 model, modified to classify and predict various vehicle features.

**Model Training**<br/>
Training for the second version of the AVIDNet model will be in the "Eval-2-Version" branch, in the "ProjectTheta.ipynb" notebook.

Conduct training by running the "Model training" section of the notebook. This will run new training for the model.

**Testing AVIDNet**<br/>
In order to test the model, run the Classifier file in Jupyter Notebook and make sure to test the images included in the project files. 

**Script Explanation**<br/>
ProjectTheta is used for model training, preprocessing, and model testing. The classifier is used to classify a single image using the 
created models.

**Model Architecture Summary**<br/>
The model architecture is based on the ResNet50 model, with the final fully connected (FC) layer altered to output vehicle type, subtype, and color.
It includes 13 layers, with 5 linear layers, 4 ReLU activation functions, and 4 dropout layers.

# Dataset Instructions:
(https://webcourses.ucf.edu/files/112308129/download?download_frd=1)

**Requirements**
```bash
pip install numpy pandas torch torchvision seaborn

