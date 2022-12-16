Los requerimientos para los frameworks y librerías utilizadas son:

pip install transformers <br>
pip install tensorflow <br>
pip install path.py <br>
pip install numpy <br>
pip install pandas <br>
pip install sklearn <br>
pip install tqdm <br>
pip install seaborn <br>

Enlace a carpeta con los pesos de entrenamiento guardados:

https://drive.google.com/drive/u/1/folders/1hGLg3aLGzcM6UlhgzYnn7an_XgVXJTw_ 

Hacer los cambio a las variables:

path =  "nuevo directorio"
df=pd.read_csv("IMDB Dataset/IMDB Dataset.csv") # nuevo directorio del dataset
checkpoint_path= "training/cp.ckpt" #nuevo directorio de entrenamiento
