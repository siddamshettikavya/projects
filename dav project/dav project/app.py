from flask import Flask, render_template, request, redirect, url_for, session
import pandas as pd
import plotly.graph_objs as go
import plotly.express as px
import os

app = Flask(__name__)
app.secret_key = "replace_this_with_a_random_key_12345"  # for session

USERNAME = "admin"
PASSWORD = "1234"

# ----------------------------- PLOT FUNCTIONS ---------------------------- #

def make_plot(df_before, df_after, col, title, yaxis_title):
    fig = go.Figure()

    fig.add_trace(go.Scatter(
        x=df_before['Date'], y=df_before[col], mode='lines',
        name=f'Before {col}'
    ))

    fig.add_trace(go.Scatter(
        x=df_after['Date'], y=df_after[col], mode='lines',
        name=f'After {col}', line=dict(dash='dash')
    ))

    fig.update_layout(
        title=title,
        yaxis_title=yaxis_title,
        margin=dict(l=20, r=20, t=40, b=20)
    )

    return fig.to_html(full_html=False, include_plotlyjs='cdn')


def combined_plot(df_before, df_after):
    fig = go.Figure()

    # Open
    fig.add_trace(go.Scatter(
        x=df_before["Date"], y=df_before["Open"],
        name="Before Open", mode="lines", line=dict(color="blue")
    ))
    fig.add_trace(go.Scatter(
        x=df_after["Date"], y=df_after["Open"],
        name="After Open", mode="lines", line=dict(color="blue", dash="dash")
    ))

    # High
    fig.add_trace(go.Scatter(
        x=df_before["Date"], y=df_before["High"],
        name="Before High", mode="lines", line=dict(color="green")
    ))
    fig.add_trace(go.Scatter(
        x=df_after["Date"], y=df_after["High"],
        name="After High", mode="lines", line=dict(color="purple", dash="dash")
    ))

    # Low
    fig.add_trace(go.Scatter(
        x=df_before["Date"], y=df_before["Low"],
        name="Before Low", mode="lines", line=dict(color="orange")
    ))
    fig.add_trace(go.Scatter(
        x=df_after["Date"], y=df_after["Low"],
        name="After Low", mode="lines", line=dict(color="cyan", dash="dash")
    ))

    # Close
    fig.add_trace(go.Scatter(
        x=df_before["Date"], y=df_before["Close"],
        name="Before Close", mode="lines", line=dict(color="red")
    ))
    fig.add_trace(go.Scatter(
        x=df_after["Date"], y=df_after["Close"],
        name="After Close", mode="lines", line=dict(color="lime", dash="dash")
    ))

    fig.update_layout(
        title="Combined Price Metrics (Before vs After)",
        margin=dict(l=20, r=20, t=40, b=20)
    )

    return fig.to_html(full_html=False, include_plotlyjs=False)

# ----------------------------- ROUTES ----------------------------- #

@app.route('/', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        username = request.form.get('username')
        password = request.form.get('password')

        if username == USERNAME and password == PASSWORD:
            session['user'] = username
            return redirect(url_for('dashboard'))
        else:
            return render_template('login.html', error="Invalid credentials")

    return render_template('login.html')


@app.route('/logout')
def logout():
    session.pop('user', None)
    return redirect(url_for('login'))


@app.route('/dashboard')
def dashboard():
    if 'user' not in session:
        return redirect(url_for('login'))
    return render_template('dashboard.html', user=session.get('user'))


@app.route("/compare", methods=["POST"])
def compare():
    before_file = request.files.get("before_csv")
    after_file = request.files.get("after_csv")

    if not before_file or not after_file:
        return "Please upload both CSV files!", 400

    before = pd.read_csv(before_file)
    after = pd.read_csv(after_file)

    before["Date"] = pd.to_datetime(before["Date"])
    after["Date"] = pd.to_datetime(after["Date"])

    open_plot = make_plot(before, after, 'Open', 'Open Price Comparison', 'Open Price')
    high_plot = make_plot(before, after, 'High', 'High Price Comparison', 'High Price')
    low_plot = make_plot(before, after, 'Low', 'Low Price Comparison', 'Low Price')
    close_plot = make_plot(before, after, 'Close', 'Close Price Comparison', 'Close Price')
    vol_plot = make_plot(before, after, 'Volume', 'Volume Comparison', 'Volume')

    combined_html = combined_plot(before, after)

    summary = {
        'before_avg_close': round(before['Close'].mean(), 2),
        'after_avg_close': round(after['Close'].mean(), 2),
        'before_avg_vol': int(before['Volume'].mean()),
        'after_avg_vol': int(after['Volume'].mean()),
    }

    return render_template(
        "compare.html",
        open_plot=open_plot,
        high_plot=high_plot,
        low_plot=low_plot,
        close_plot=close_plot,
        vol_plot=vol_plot,
        combined_plot=combined_html,
        summary=summary
    )


if __name__ == "__main__":
    app.run(debug=True)
