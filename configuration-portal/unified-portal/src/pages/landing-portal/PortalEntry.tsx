import { Link } from 'react-router-dom';

function PortalEntry({ title, location, background }: { title: string, location: string | Object, background: string }) {
    return (
        <div className="portal-entry">
            <Link className="portal-entry-inner-wrapper" to={location}>
                <div className="portal-entry-bg-img" style={{ backgroundImage: `url(${background})` }}></div>
                <div className="portal-entry-title">{title}
                </div>
            </Link>
        </div>
    );
}

export default PortalEntry;