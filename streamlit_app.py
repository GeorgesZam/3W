import streamlit as st
import pandas as pd
import matplotlib.pyplot as plt
import os

# Fonction pour charger les données 
def load_data(filepath):
    if os.path.exists(filepath):
        return pd.read_csv(filepath)
    else:
        st.error("Le fichier de données n'existe pas encore. Veuillez lancer l'Arduino pour commencer la collecte de données.")
        return pd.DataFrame()

# Configuration de la page Streamlit
st.set_page_config(page_title="Visualisation des Données Arduino", layout="wide")

# Titre de l'application
st.title("Visualisation des Données Collectées par Arduino")

# Chargement du fichier CSV via drag and drop
uploaded_file = st.file_uploader("Choisissez un fichier CSV", type="csv")

# Charger les données si un fichier est uploadé
if uploaded_file is not None:
    df = pd.read_csv(uploaded_file)

    # Si des données sont présentes, afficher les graphiques
    if not df.empty:
        st.write("## Aperçu des Données Collectées")
        st.dataframe(df)

        # Création des graphiques
        st.write("## Visualisation des Capteurs")

        # Graphique de la température
        fig, ax = plt.subplots()
        ax.plot(df['temperature'], label='Température (C)', color='r')
        ax.set_ylabel("Température (C)")
        ax.set_title("Évolution de la Température")
        st.pyplot(fig)

        # Graphique de l'humidité
        fig, ax = plt.subplots()
        ax.plot(df['humidity'], label='Humidité (%)', color='b')
        ax.set_ylabel("Humidité (%)")
        ax.set_title("Évolution de l'Humidité")
        st.pyplot(fig)

        # Graphique de la luminosité
        fig, ax = plt.subplots()
        ax.plot(df['luminosity'], label='Luminosité (Analogique)', color='g')
        ax.set_ylabel("Luminosité (Analogique)")
        ax.set_title("Évolution de la Luminosité")
        st.pyplot(fig)

else:
    st.warning("Aucune donnée disponible. Assurez-vous de charger un fichier CSV pour visualiser les données.")
