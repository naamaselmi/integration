import sys
import smtplib
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart

def send_email(owner_name, brand, model, year, plate, state, maintenance_history, recipient_email):
    sender_email = "6ysfcrybaby9@gmail.com"
    sender_password = "bsxa kzmi hdzn gwyk"  # Use an app password for security

    # Email content
    subject = "Informations sur votre véhicule"
    body = f"""
    Bonjour {owner_name},

    Votre voiture de marque {brand}, modèle {model}, année {year}, immatriculée {plate} est actuellement dans l'état suivant : {state}.

    Voici l'historique des entretiens effectués : {maintenance_history}.

    Merci de votre confiance.
    """

    # Create email message
    message = MIMEMultipart()
    message["From"] = sender_email
    message["To"] = recipient_email
    message["Subject"] = subject
    message.attach(MIMEText(body, "plain"))

    # Connect to the server and send the email
    try:
        with smtplib.SMTP_SSL("smtp.gmail.com", 465) as server:
            server.login(sender_email, sender_password)
            server.sendmail(sender_email, recipient_email, message.as_string())
        print("Email sent successfully")
    except Exception as e:
        print(f"Failed to send email: {e}", file=sys.stderr)

if __name__ == "__main__":
    if len(sys.argv) != 9:
        print("Usage: sendmail.py <owner_name> <brand> <model> <year> <plate> <state> <maintenance_history> <email>", file=sys.stderr)
        sys.exit(1)

    owner_name = sys.argv[1]
    brand = sys.argv[2]
    model = sys.argv[3]
    year = sys.argv[4]
    plate = sys.argv[5]
    state = sys.argv[6]
    maintenance_history = sys.argv[7]
    recipient_email = sys.argv[8]

    send_email(owner_name, brand, model, year, plate, state, maintenance_history, recipient_email)
